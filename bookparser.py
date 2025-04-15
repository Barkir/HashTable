alignment = int(input())
file = open("books/HolyBible.txt").read()
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
        # words.append(i.ljust(32, '0'))
file = open("info/parsed.bible", "w+")
for i in words:
    file.write(i + '\n')

file.close()

