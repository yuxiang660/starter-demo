import pandas

if __name__ == "__main__":
    data = pandas.read_json('input.json')
    print(data)
    data.to_csv("output.csv", index=False)
    result = pandas.read_csv("output.csv")
    print(result)
