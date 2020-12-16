input_file = open('input.txt', 'r')
Lines = input_file.readlines()
Lines.append('0\n')

output_file = open('modif_array.h', 'w')

arraySize = str(len(Lines))

arrayDecl = 'constexpr std::array<unsigned int,ARRAY_SIZE> numbers {\n'

output_file.write('#pragma once\n')
output_file.write('#include <array>\n')
output_file.write('constexpr unsigned int ARRAY_SIZE = ' + arraySize + ';\n')
output_file.write(arrayDecl)
for line in Lines[:-1] :
    output_file.write(line + ',')
output_file.write(Lines[-1])
output_file.write('};\n')

input_file.close()
output_file.close()
