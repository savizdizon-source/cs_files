from PIL import Image

# 4.

def convert(color):
    if color == "R": return (237, 28, 36, 255)
    elif color == "B": return (0, 0, 0, 255)
    elif color == "Y": return (255, 242, 0, 255)
    else: return color

input_text_file = open("C:\\Users\\saviz\\cs_files\\input.txt", "r")
lines = input_text_file.readlines() # the height of the image = number of lines
width, height = lines[0].count(" "), len(lines)
output_image_file = Image.new("RGB", (width, height))

for y in range(height):
    line = lines[y]
    pixels = line.split()
    for x in range(width):
        pixel = pixels[x]
        pixel = convert(pixel)
        output_image_file.putpixel((x, y), pixel)

output_image_file.save("output.png")

output_image_file.close()
input_text_file.close()


    