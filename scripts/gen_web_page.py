import argparse


def html_to_header(input_html_file, output_header_file):
    with open(input_html_file, 'r', encoding='utf-8') as html_file:
        html_content = html_file.read()
    
    # Escape special characters
    
    with open(output_header_file, 'w', encoding='utf-8') as header_file:
        header_file.write('const char setupPage[] PROGMEM = R"rawliteral(\n')
        header_file.write(html_content)
        header_file.write('\n)rawliteral";\n\n')

def main():
    parser = argparse.ArgumentParser()
    parser.add_argument('input_html_file', help='Input HTML file')
    parser.add_argument('output_header_file', help='Output header file')
    args = parser.parse_args()
    html_to_header(args.input_html_file, args.output_header_file)

if __name__ == '__main__':
    main()

