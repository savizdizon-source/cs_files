public class hw1_2 {
    public static void main(String[] args) {
        test("0", 10);
        test("FFFFFFFF", 16);
        test("-1", 10);
        test(Integer.toString(Integer.MIN_VALUE), 10);
        // String num = "8";
        // String convertTo = "octal";

        // int converted;

        // switch (convertTo) {
        //     case "binary":
        //         converted = convert(num, 2);
        //         System.out.println("Binary: " + converted);
        //         break;
        //     case "decimal":
        //         converted = convert(num, 10);
        //         System.out.println("Decimal: " + converted);
        //         break;
        //     case "octal":
        //         converted = convert(num, 8);
        //         System.out.println("Octal: " + converted);
        //         break;
        //     case "hexadecimal":
        //         converted = convert(num, 16);
        //         System.out.println("Hexadecimal: " + converted);
        //         break;
        //     default:
        //         throw new AssertionError();
    }

    static void test(String num, int base) {
        int converted = convert(num, base);
        System.out.println("Number: " + num + "\nConverting to: " + base + 
                            "\nConverted num: " + converted);
    }

    // Number converter
    static int convert(String num, int base) {
        int output;
        switch (base) {
            case 2 -> output = Integer.parseInt(num, 2);
            case 10 -> output = Integer.parseInt(num, 10); 
            case 8 -> output = Integer.parseInt(num, 8);
            case 16 -> output = Integer.parseInt(num, 16);
            default -> output = -1;
        }

        return output;
    }
}