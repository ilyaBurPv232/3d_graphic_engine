import subprocess


def is_code_line(line):
    """Проверяет, является ли строка кодом (а не комментарием или пустой)."""
    line = line.strip()

    # Пустая строка → не код
    if not line:
        return False

    # Строка начинается с комментария → не код
    comment_prefixes = ["//", "/*", "*", "#"]
    if any(line.startswith(prefix) for prefix in comment_prefixes):
        return False

    return True


def count_lines_of_code():
    """Считает строки кода и создает файл с объединенным кодом."""
    result = subprocess.run(['git', 'ls-files'], capture_output=True, text=True)
    files = [f for f in result.stdout.splitlines() if f.endswith(('.h', '.cpp', '.fsh', '.vsh'))]

    total_lines = 0
    with open('all_code.txt', 'w', encoding='utf-8') as output_file:
        for file in files:
            with open(file, 'r', encoding='utf-8', errors='ignore') as f:
                file_lines = [line for line in f if is_code_line(line)]
                total_lines += len(file_lines)

                # Записываем название файла как разделитель
                output_file.write(f"\n\n// ====== {file} ====== //\n\n")
                # Записываем код файла
                output_file.writelines(file_lines)

    print(f"Total lines of **code** (no comments/empty lines): {total_lines}")
    print("All code has been saved to 'all_code.txt'")


if __name__ == "__main__":
    count_lines_of_code()