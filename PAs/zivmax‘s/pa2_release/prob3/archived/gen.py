def generate_text(pairs):
    text = ""
    counter = 0
    pairs.reverse()
    for pair in pairs:
        for _ in range(pair[1]):
            text += chr(97 + counter % 26) * pair[0]  # chr(97) is 'a'
            counter += 1
    return text


# Test the function
pairs = [(1, 6), (2, 3), (5, 4), (6, 2), (9, 1), (100, 1)]
print(generate_text(pairs))
