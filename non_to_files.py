
import sys

def main(non_file_path):
    cols = []
    rows = []
    output = None  # will be either cols or rows, depending on header

    for line in open(non_file_path):
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
                constrains = line.strip().split(',')
                assert all([elm.isdigit() for elm in constrains])
                output.append(constrains)
    open('cols.txt', 'w').write(encode(cols))
    open('rows.txt', 'w').write(encode(rows))

def encode(con):
    return '\n-1\n'.join(['\n'.join(l) for l in con]) + '\n-1'

if __name__ == '__main__':
    main(sys.argv[1])

