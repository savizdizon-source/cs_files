import java.io.ByteArrayOutputStream;
import java.io.IOException;
import java.nio.charset.StandardCharsets;
import java.nio.file.Files;
import java.nio.file.Paths;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.TreeSet;
/*
 * Toy MIPS disassembler. Java 8+. Reads either output file from Assembler.java and
 * writes a MIPS assembly file that Assembler.java can assemble again.
 *
 * javac Assembler.java Disassembler.java
 * java Disassembler fizzbuzz.mhc-bin    -> writes fizzbuzz_from_bin.asm
 * java Disassembler fizzbuzz.mhc-ascii  -> writes fizzbuzz_from_ascii.asm
 * java Disassembler                     -> same as the first line, reading fizzbuzz.mhc-bin
 * The input format is detected from the file contents, not the file name. The output
 * is written beside the input; it never overwrites the original fizzbuzz.asm.
 * If disassembly fails, no output file is written.
 *
 * Input formats (see the header comment of Assembler.java for the full layout):
 *   .mhc-bin    20-byte big-endian header (magic 'M','H','C',1, then .text address and
 *               size, .data address and size), followed by the .text bytes and the
 *               .data bytes.
 *   .mhc-ascii  Text listing. Each row under ".text @ ..." begins with a 32-bit binary
 *               word. Each row under ".data @ ..." carries its bytes in the ".byte"
 *               list. Everything after the first 32 bits of a .text row is ignored.
 *
 * Reconstruction rules:
 * - Instructions and data stay separate: .text bytes become instructions, .data bytes
 *   become .asciiz strings.
 * - Comments, original label names, and pseudoinstruction spellings are not recovered.
 *   The first instruction is labeled main, other branch/jump targets are labeled
 *   L1, L2, ... in address order, and data strings are labeled str0, str1, ...
 * - A lui + ori pair that builds the address of a data string is written as la.
 *   An all-zero word is written as nop. Everything else uses the real instruction.
 * - Verification: after writing the file, the program assembles the reconstructed
 *   text with Assembler.assemble and compares its .text and .data bytes with the
 *   input. It prints whether they match.
 *
 * Supported instructions: addi, addiu, addu, ori, lui, div, mfhi, sll,
 *   beq, bne, j, jal, jr, lw, sw, syscall, nop (and la, as described above).
 *
 * Errors: an unrecognized file, a size that does not match its header, a bad row
 * (with its line number), an unsupported instruction word (with its address), a
 * branch outside .text, or .data that is not .asciiz text prints a message and
 * writes no file.
 *
 * Limitations:
 * - Only the default addresses are accepted: .text = 0x00400000, .data = 0x10010000.
 * - .data must be a series of 0-terminated ASCII strings, as .asciiz produces.
 * - Instruction words must be exactly what Assembler.java can produce. A word with
 *   unused fields set (for example addu with a nonzero shift amount) is rejected.
 */
public class Disassembler {
    static final String DEFAULT_INPUT = "fizz_buzz.mhc-bin";
    private static final String[] REGISTER_NAMES = {
    "zero", "at", "v0", "v1", "a0", "a1", "a2", "a3",
    "t0", "t1", "t2", "t3", "t4", "t5", "t6", "t7",
    "s0", "s1", "s2", "s3", "s4", "s5", "s6", "s7",
    "t8", "t9", "k0", "k1", "gp", "sp", "fp", "ra"
    };

    static class Image {
        final byte[] text, data;
        Image(byte[] text, byte[] data) {
            this.text = text;
            this.data = data;
        }
    }

    public static void main(String[] args) {
        String input = args.length > 0 ? args[0] : DEFAULT_INPUT;
        try {
            byte[] file = Files.readAllBytes(Paths.get(input));
            boolean binary = isBinary(file);
            Image image = binary ? readBinary(file) : readAscii(new String(file, StandardCharsets.US_ASCII));
            String assembly = disassemble(image);

            String output = outputBase(input) + (binary ? "_from_bin.asm" : "_from_ascii.asm");
            Files.write(Paths.get(output), assembly.getBytes(StandardCharsets.US_ASCII));
            // println supplies the last newline; preserve the listing byte for byte.
            System.out.println(assembly.substring(0, assembly.length() - 1));
            System.out.println("Wrote " + output);
            verify(image, assembly);
        } catch (IllegalArgumentException e) {
            System.out.println("Disassembly failed: " + e.getMessage());
        } catch (IOException e) {
            System.out.println("File error: " + e.getClass().getSimpleName() + " - " + e.getMessage());
        }
    }

    // Assemble the reconstructed program again and compare it with the input bytes.
    static void verify(Image image, String assembly) {
        try {
            Assembler.Result rebuilt = Assembler.assemble(assembly);
            if (Arrays.equals(rebuilt.text, image.text) && Arrays.equals(rebuilt.data, image.data)) {
                System.out.println("Verified: assembling the reconstructed program gives the same .text and .data bytes.");
            } else {
                System.out.println("VERIFY FAILED: the reconstructed program assembles to different bytes.");
            }
        } catch (IllegalArgumentException e) {
            System.out.println("VERIFY FAILED: the reconstructed program does not assemble: " + e.getMessage());
        }
    }

    // Output files share the input's name; only the extension changes.
    static String outputBase(String input) {
        int dot = input.lastIndexOf('.');
        int slash = Math.max(input.lastIndexOf('/'), input.lastIndexOf('\\'));
        return dot > slash + 1 ? input.substring(0, dot) : input;
    }

    static boolean isBinary(byte[] file) {
        return file.length >= 4 && file[0] == 'M' && file[1] == 'H' && file[2] == 'C' && file[3] == 1;
    }

    static Image readBinary(byte[] file) {
        if (file.length < Assembler.HEADER_BYTES) fail("The file is shorter than its 20-byte header.");
        int textSize = word(file, 8);
        int dataSize = word(file, 16);
        if (textSize < 0 || dataSize < 0 || Assembler.HEADER_BYTES + (long) textSize + dataSize != file.length) {
            fail("The file length does not match the sizes in its header.");
        }
        checkSections(word(file, 4), textSize, word(file, 12));
        int dataStart = Assembler.HEADER_BYTES + textSize;
        return new Image(Arrays.copyOfRange(file, Assembler.HEADER_BYTES, dataStart),
        Arrays.copyOfRange(file, dataStart, file.length));
    }

    static Image readAscii(String contents) {
        if (!contents.startsWith("=== MHC-ASCII")) {
            fail("Not an MHC file: expected the MHC binary header or an MHC-ASCII listing.");
        }
        ByteArrayOutputStream text = new ByteArrayOutputStream();
        ByteArrayOutputStream data = new ByteArrayOutputStream();
        int textBase = 0, textSize = -1, dataBase = Assembler.DATA_BASE, dataSize = 0;
        boolean inSection = false, inData = false;
        String[] lines = contents.split("\\r\\n|\\r|\\n");
        for (int i = 0; i < lines.length; i++) {
            try {
                String line = lines[i].trim();
                if (line.isEmpty() || line.startsWith("===") || line.startsWith("Byte order") || line.startsWith("[")) continue;
                if (line.startsWith(".text @") || line.startsWith(".data @")) {
                    String[] p = line.split("\\s+");
                    if (p.length != 6 || !p[2].startsWith("0x") || !p[3].equals("-") || !p[5].equals("bytes")) {
                        fail("Expected a header like \".text @ 0x00400000 - 180 bytes\".");
                    }
                    int base = (int) Long.parseLong(p[2].substring(2), 16);
                    int size = Integer.parseInt(p[4]);
                    inSection = true;
                    inData = p[0].equals(".data");
                    if (inData) {
                        dataBase = base;
                        dataSize = size;
                    } else {
                        textBase = base;
                        textSize = size;
                    }
                    continue;
                }
                if (!inSection) fail("A row appears before any .text or .data header.");
                if (inData) {
                    String[] parts = line.split(" # ");
                    if (parts.length < 2 || !parts[1].startsWith(".byte ")) fail("Expected a .byte list in this data row.");
                    for (String value : parts[1].substring(6).split(",")) {
                        int b = Integer.parseInt(value.trim());
                        if (b < 0 || b > 255) fail("Byte value out of range: " + b);
                        data.write(b);
                    }
                } else {
                    if (line.length() < 32 || !line.substring(0, 32).matches("[01]{32}")
                    || (line.length() > 32 && line.charAt(32) != ' ')) {
                        fail("Expected a 32-bit binary word at the start of this row.");
                    }
                    int word = (int) Long.parseLong(line.substring(0, 32), 2);
                    for (int shift = 24; shift >= 0; shift -= 8) text.write(word >>> shift);
                }
            } catch (NumberFormatException e) {
                throw new IllegalArgumentException("Line " + (i + 1) + ": Invalid number: " + e.getMessage());
            } catch (IllegalArgumentException e) {
                throw new IllegalArgumentException("Line " + (i + 1) + ": " + e.getMessage());
            }
        }
        if (textSize < 0) fail("No .text section found.");
        if (text.size() != textSize) fail(".text has " + text.size() + " bytes but its header says " + textSize + ".");
        if (data.size() != dataSize) fail(".data has " + data.size() + " bytes but its header says " + dataSize + ".");
        checkSections(textBase, textSize, dataBase);
        return new Image(text.toByteArray(), data.toByteArray());
    }

    private static void checkSections(int textBase, int textSize, int dataBase) {
        if (textBase != Assembler.TEXT_BASE) fail(".text must start at 0x" + Assembler.hex(Assembler.TEXT_BASE) + ", not 0x" + Assembler.hex(textBase) + ".");
        if (dataBase != Assembler.DATA_BASE) fail(".data must start at 0x" + Assembler.hex(Assembler.DATA_BASE) + ", not 0x" + Assembler.hex(dataBase) + ".");
        if (textSize == 0) fail("No instructions found.");
        if (textSize % 4 != 0) fail(".text size must be a multiple of 4 bytes, not " + textSize + ".");
    }

    static String disassemble(Image image) {
        int end = Assembler.TEXT_BASE + image.text.length;

        // Pass 1: find every branch/jump target so each one can get a label.
        TreeSet<Integer> targets = new TreeSet<>();
        for (int address = Assembler.TEXT_BASE; address < end; address += 4) {
            int word = word(image.text, address - Assembler.TEXT_BASE);
            if (!hasTarget(word)) continue;
            int target = target(word, address);
            if (target < Assembler.TEXT_BASE || target > end) {
                fail("Instruction at 0x" + Assembler.hex(address) + " jumps to 0x" + Assembler.hex(target) + ", outside .text.");
            }
            targets.add(target);
        }
        Map<Integer, String> labels = new HashMap<>();
        labels.put(Assembler.TEXT_BASE, "main");
        int number = 1;
        for (int target : targets) {
            if (!labels.containsKey(target)) labels.put(target, "L" + number++);
        }

        // .data must be 0-terminated ASCII strings; remember where each one starts.
        List<String> strings = new ArrayList<>();
        Map<Integer, String> dataLabels = new HashMap<>();
        StringBuilder current = new StringBuilder();
        int start = 0;
        for (int i = 0; i < image.data.length; i++) {
            int b = image.data[i] & 255;
            if (b != 0) {
                current.append(escaped(b));
                continue;
            }
            dataLabels.put(Assembler.DATA_BASE + start, "str" + strings.size());
            strings.add(current.toString());
            current.setLength(0);
            start = i + 1;
        }
        if (start != image.data.length) fail("The .data bytes do not end with a 0 byte, so they cannot be written as .asciiz text.");

        // Pass 2: write the program.
        StringBuilder output = new StringBuilder();
        if (!strings.isEmpty()) {
            appendLine(output, ".data");
            for (int i = 0; i < strings.size(); i++) {
                appendLine(output, "str" + i + ": .asciiz \"" + strings.get(i) + "\"");
            }
        }
        appendLine(output, ".text");
        appendLine(output, ".globl main");
        for (int address = Assembler.TEXT_BASE; address < end; address += 4) {
            if (labels.containsKey(address)) appendLine(output, labels.get(address) + ":");
            String pair = loadAddress(image.text, address, end, labels, dataLabels);
            if (pair != null) {
                appendLine(output, "    " + pair);
                address += 4; // The ori half of the pair is already written.
                continue;
            }
            appendLine(output, "    " + instruction(word(image.text, address - Assembler.TEXT_BASE), address, labels));
        }
        if (labels.containsKey(end)) appendLine(output, labels.get(end) + ":");
        return output.toString();
    }

    private static void appendLine(StringBuilder output, String line) {
        output.append(line).append('\n');
    }

    // lui + ori on the same register that builds a data string's address is one la.
    private static String loadAddress(byte[] text, int address, int end, Map<Integer, String> labels,
    Map<Integer, String> dataLabels) {
        if (address + 4 >= end || labels.containsKey(address + 4)) return null;
        int first = word(text, address - Assembler.TEXT_BASE);
        int second = word(text, address + 4 - Assembler.TEXT_BASE);
        int register = (first >>> 16) & 31;
        if ((first >>> 21) != (15 << 5)) return null;
        if ((second >>> 26) != 13 || ((second >>> 21) & 31) != register || ((second >>> 16) & 31) != register) return null;
        String name = dataLabels.get(((first & 0xFFFF) << 16) | (second & 0xFFFF));
        return name == null ? null : "la " + register(register) + ", " + name;
    }

    private static boolean hasTarget(int word) {
        int opcode = word >>> 26;
        return opcode == 2 || opcode == 3 || opcode == 4 || opcode == 5;
    }

    // Branches are relative to the next instruction; jumps keep the next instruction's top 4 bits.
    private static int target(int word, int address) {
        int opcode = word >>> 26;
        if (opcode == 4 || opcode == 5) return address + 4 + (short) word * 4;
        return ((address + 4) & 0xF0000000) | ((word & 0x03FFFFFF) << 2);
    }

    // Reverse of Assembler.encode for one word. Unused fields must be zero.
    private static String instruction(int word, int address, Map<Integer, String> labels) {
        int opcode = word >>> 26;
        int rs = (word >>> 21) & 31, rt = (word >>> 16) & 31, rd = (word >>> 11) & 31;
        int shift = (word >>> 6) & 31, function = word & 63;
        int immediate = word & 0xFFFF, signed = (short) word;
        if (opcode == 0) {
            if (word == 0) return "nop";
            if (function == 0x21 && shift == 0) return "addu " + register(rd) + ", " + register(rs) + ", " + register(rt);
            if (function == 0x1A && rd == 0 && shift == 0) return "div " + register(rs) + ", " + register(rt);
            if (function == 0x10 && rs == 0 && rt == 0 && shift == 0) return "mfhi " + register(rd);
            if (function == 0 && rs == 0) return "sll " + register(rd) + ", " + register(rt) + ", " + shift;
            if (function == 8 && rt == 0 && rd == 0 && shift == 0) return "jr " + register(rs);
            if (word == 0x0000000C) return "syscall";
        } else if (opcode == 8 || opcode == 9) {
            return (opcode == 8 ? "addi " : "addiu ") + register(rt) + ", " + register(rs) + ", " + signed;
        } else if (opcode == 13) {
            return "ori " + register(rt) + ", " + register(rs) + ", " + immediate;
        } else if (opcode == 15 && rs == 0) {
            return "lui " + register(rt) + ", " + immediate;
        } else if (opcode == 4 || opcode == 5) {
            return (opcode == 4 ? "beq " : "bne ") + register(rs) + ", " + register(rt) + ", " + labels.get(target(word, address));
        } else if (opcode == 2 || opcode == 3) {
            return (opcode == 2 ? "j " : "jal ") + labels.get(target(word, address));
        } else if (opcode == 35 || opcode == 43) {
            return (opcode == 35 ? "lw " : "sw ") + register(rt) + ", " + signed + "(" + register(rs) + ")";
        }
        throw new IllegalArgumentException("Unsupported instruction word 0x" + Assembler.hex(word)
        + " at address 0x" + Assembler.hex(address) + ".");
    }

    private static String register(int number) {
        return "$" + REGISTER_NAMES[number];
    }

    // One data byte as it must be written inside an .asciiz string.
    private static String escaped(int b) {
        switch (b) {
            case '\n': return "\\n";
            case '\r': return "\\r";
            case '\t': return "\\t";
            case '\\': return "\\\\";
            case '"': return "\\\"";
            default:
                if (b < 32 || b > 126) fail("A .data byte (" + b + ") cannot be written as .asciiz text.");
                return String.valueOf((char) b);
        }
    }

    private static int word(byte[] bytes, int offset) {
        int word = 0;
        for (int i = 0; i < 4; i++) word = (word << 8) | (bytes[offset + i] & 255);
        return word;
    }

    private static void fail(String message) {
        throw new IllegalArgumentException(message);
    }
}