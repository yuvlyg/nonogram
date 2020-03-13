
import sys

def main(path):
    cols = []
    rows = []
    non_file = open(path)
    output = None
    for line in non_file:
        if not line.strip():
            continue
        if line.strip().startswith('goal'):
            break
        if line.strip() == 'columns':
            output = cols
        elif line.strip() == 'rows':
            output = rows
        else:
            if output is not None:
                output.append(line.strip().split(','))
    open('cols.txt', 'w').write(encode(cols))
    open('rows.txt', 'w').write(encode(rows))

def encode(con):
    return '\n-1\n'.join(['\n'.join(l) for l in con]) + '\n-1'

if __name__ == '__main__':
    main(sys.argv[1])

