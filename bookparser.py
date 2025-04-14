file = open("books/HarryPotter.txt").read()
for comma in "!@#$%^&*()_+[]{}\";:<>.,`'":
    file = file.replace(comma, "")

file = file.split("\n")

words = []

for line in file:
    line = line.split()
    for i in line:
        words.append(i.zfill(16))
file = open("parsed.lib", "w+")
for i in words:
    file.write(i + '\n')

file.close()

