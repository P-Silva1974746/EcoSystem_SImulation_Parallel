def extract_first_table(input_file, output_file):
    with open(input_file, "r") as f:
        lines = f.readlines()

    out = []
    i = 0
    n = len(lines)

    while i < n:
        line = lines[i]

        # Copy generation line
        if line.startswith("Generation"):
            out.append(line)
            i += 1
            continue

        # Start divider
        if line.strip().startswith("-"):
            divider = line.strip().split()[0] + "\n"   # only first block of ----
            out.append(divider)
            i += 1

            # Process table lines
            while i < n and not lines[i].strip().startswith("-"):
                row = lines[i]

                # Find first two '|'
                try:
                    first = row.index("|")
                    second = row.index("|", first + 1)
                    cleaned = row[first:second+1] + "\n"
                    out.append(cleaned)
                except ValueError:
                    pass  # ignore malformed rows

                i += 1

            # End divider
            if i < n and lines[i].strip().startswith("-"):
                end_div = lines[i].strip().split()[0] + "\n"
                out.append(end_div)
                i += 1

            # blank line between generations
            out.append("\n")
            continue

        i += 1

    with open(output_file, "w") as f:
        f.writelines(out)


# Example:
extract_first_table("ecosystem_examples/allgen100x100", "ecosystem_examples/allgen100x100_v2")
