alignment = int(input())
harry = "books/HarryPotter.txt"
bible = "books/HolyBible.txt"

harry_parse = "info/parsed16.harry"
bible_parse = "info/parsed16.bible"

file = open(harry).read()

for comma in "!@#$%^&*()_+[]{}\";:<>.,`'":
    file = file.replace(comma, "")

file = file.split("\n")

words = []

for line in file:
    line = line.split()
    for i in line:
        if (alignment == 32):
            words.append(i.ljust(32, '0'))
        elif (alignment == 16):
            words.append(i.ljust(16, '0'))
        else:
            words.append(i)
file = open(harry_parse, "w+")
for i in words:
    file.write(i + '\n')

file.close()

