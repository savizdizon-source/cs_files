from PIL import Image

# 3. Write a program that reads an image and prints its pixel values.

def convert(color):
    if color == "(237, 28, 36, 255)": return "R"
    elif color == "(0, 0, 0, 255)": return "B"
    elif color == "(255, 242, 0, 255)": return "Y"
    else: return color

output_file = open("output.txt", "w")

image_file = Image.open("C:\\Users\\saviz\\cs_files\\cs_240\\assignment1\\smiley.png")
image_file.load()

width, height = image_file.size # two value tuple

for y in range(height):
    for x in range(width):
        color = str(image_file.getpixel((x, y)))
        color = convert(color)
        output_file.write(color)
        output_file.write(" ")
    output_file.write("\n")

output_file.close()


    