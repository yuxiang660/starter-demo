import csv
with open('sd_sd.csv', 'r') as file:
    reader = csv.reader(file)
    for row in reader:
        print(row)