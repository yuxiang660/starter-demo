import re

def extract_numbers():
    numbers = re.findall(r"\d*\.\d*|\d+", "0.1+0.2+99")
    print(numbers)
    num_list = []
    for n in numbers:
        num = float(n) if '.' in n else (float(n) + 0.8)
        num_list.append(num)
    num_list_sort = list(set(num_list))
    num_list_sort.sort()
    if num_list == num_list_sort:
        print("true")
    else:
        print("false")
    print(num_list)
    print(num_list_sort)

def get_boards(board):
    if '.' in board: return [float(board)]
    boards = []
    for i in range(8):
        b = float(board) + 0.1 * i
        boards.append(b)
    return boards

def extend_boards(pre_board, post_board):
    if '.' not in pre_board and '.' not in post_board:
        begin = int(pre_board) + 1
        end = int(post_board) + 1
        boards = []
        for i in range(begin, end):
            boards.extend([i + x * 0.1 for x in range(8)])
        return boards

    if '.' not in post_board and '.' in post_board:
        begin = int(pre_board) + 1
        end = int(float(post_board))
        boards = []
        for i in range(begin, end):
            boards.extend([i + x * 0.1 for x in range(8)])
        for b in range(end + 0.0, float(post_board) + 0.1, 0.1):
            boards.append(b)
        return boards

    else:
        end_board = float(post_board)

def extend_string():
    split_config = re.split(r'(\+|\-)', '0.1+0.2-0.4-2')
    print(split_config)
    minus_position = []
    for index, c in enumerate(split_config):
        if c == '-': minus_position.append(index)
    print(minus_position)
    extended_boards = []
    i = 0
    while i < len(split_config):
        if split_config[i] == '+': continue
        if c == '-':
            extended_boards.extend(extend_boards(split_config[i-1], split_config[i+1]))
            i += 1
        extended_boards.extend(get_boards(split_config[i]))
        i += 1
    print(extended_boards)

def is_domain(word):
    if word != '+' and word != '-': return True
    return False

def add_domains(boards_dict, word):
    if '.' not in word:
        board = int(word)
        domains = [x for x in range(8)]
    else:
        board = int(float(word))
        domains = [int(float(word) * 10 % 10)]
    domains_list = boards_dict.get(board, list())
    domains_list = sorted(set(domains_list + domains))
    boards_dict[board] = domains_list
    return boards_dict

def add_domains_between_section(boards_dict, left_word, right_word):
    if '.' not in left_word or [int(float(left_word) * 10 % 10)] == 7:
        left_board = int(left_word) + 1
        left_domain = 0
    else:
        left_board = int(float(left_word))
        left_domain = [int(float(left_word) * 10 % 10)] + 1
    if '.' not in right_word or [int(float(right_word) * 10 % 10)] == 0:
        right_board = int(right_word) - 1
        right_domain= 7
    else:
        right_board = int(float(right_word))
        right_domain = [int(float(right_word) * 10 % 10)] - 1
    return boards_dict

def parse_boards():
    boards_dict = {}
    words = re.split(r'(\+|\-)', '0.1+0.2+2+3.1')
    for word in words:
        if is_domain(word): boards_dict = add_domains(boards_dict, word)
    minus_position = []
    for index, word in enumerate(words):
        if word == '-': minus_position.append(index)
    for i in minus_position:
        boards_dict = add_domains_between_section(boards_dict, words[i-1], words[i+1])
    return boards_dict

def add_domain_indexes(domain_indexes, word):
    if '.' not in word:
        board = int(word)
        domains = [board * 8 + x for x in range(8)]
    else:
        board = int(float(word))
        domains = [board * 8 + int(float(word) * 10 % 10)]
    domain_indexes = sorted(set(domain_indexes + domains))
    return domain_indexes

def add_domain_indexes_between_section(domain_indexes, left_word, right_word):
    if '.' not in left_word:
        board = int(left_word)
        left_domain = (board + 1) * 8
    else:
        board = int(float(left_word))
        left_domain = board * 8 + int(float(left_word) * 10 % 10) + 1
    if '.' not in right_word:
        board = int(right_word)
        right_domain = board * 8
    else:
        board = int(float(right_word))
        right_domain = board * 8 + int(float(right_word) * 10 % 10)
    print(left_domain)
    print(right_domain)
    domain_indexes = sorted(set(domain_indexes + [x for x in range(left_domain, right_domain)]))
    return domain_indexes

def parse_boards_config():
    domain_indexes = []
    words = re.split(r'(\+|\-)', '0.1+0.2-0.5+2-4+5.4-5.6')
    for word in words:
        if is_domain(word): domain_indexes = add_domain_indexes(domain_indexes, word)
    minus_position = []
    for index, word in enumerate(words):
        if word == '-': minus_position.append(index)
    for i in minus_position:
        domain_indexes = add_domain_indexes_between_section(domain_indexes, words[i-1], words[i+1])
    return domain_indexes

def main():
    print("Regex Extract Test")
    # boards = parse_boards()
    # for board, domains in boards.items():
    #     print(f'board: {board}, ', domains)
    indexes = parse_boards_config()
    print(indexes)

if __name__ == "__main__":
    main()
