import os
import numpy as np
import glob

def two_atts(txt):
    first = False
    pos = 0
    bracket_balance = 0
    comma_pos = -1
    while pos < len(txt):
        if txt[pos] == '(':
            bracket_balance += 1
        if txt[pos] == ')':
            bracket_balance -= 1

        if bracket_balance == 1 and not first:
            opos = pos
            first = True

        if bracket_balance == 1 and first:
            if txt[pos] == ',':
                comma_pos = pos

        if bracket_balance == 0 and first:
            cpos = pos
            break

        pos += 1

    return opos, comma_pos, cpos



def two_attributes(txt):
    opos, comma_pos, cpos = two_atts(txt)
    if cops == len(txt):
        return "", ""
    return txt[opos+1:comma_pos].strip(), txt[comma_pos+1:cpos].strip()


def do_what_is_possible(txt):
    out = []
    for line in txt.splitlines():

        if 'TEST_F' in line:
            num_ws = line.find('TEST_F')
            first, second = two_attributes(txt)
            if first and second:
                o = ' '*num_ws + 'TEST_CASE("{name:s}", "{tag:s}")'.format(
                    name=second.strip(),
                    tag=first.strip()) + " {"
                out.append(o)
        elif 'EXPECT_EQ' in line:
            num_ws = line.find('EXPECT_EQ')
            first, second = two_attributes(txt)
            if first and second:
                o = ' '*num_ws + 'CHECK({first:s} == {second:s})'.format(
                    first=first,
                    second=second)
                out.append(o)
        elif 'ASSERT_EQ' in line:
            num_ws = line.find('ASSERT_EQ')
            first, second = two_attributes(txt)
            if first and second:
                o = ' '*num_ws + 'REQUIRE({first:s} == {second:s})'.format(
                    first=first,
                    second=second)
                out.append(o)
        elif 'EXPECT_NE' in line:
            num_ws = line.find('EXPECT_NE')
            first, second = two_attributes(txt)
            if first and second:
                o = ' '*num_ws + 'CHECK({first:s} != {second:s})'.format(
                    first=first,
                    second=second)
                out.append(o)
        elif 'EXPECT_TRUE' in line:
            num_ws = line.find('EXPECT_TRUE')
            o = ' '*num_ws + 'CHECK' + line[num_ws + 11:]
            out.append(o)
        else:
            out.append(line)
    out = "\n".join(out)
    return out

gtest_dir = os.path.join('tests')

srcs = []
for src_path in glob.glob(os.path.join(gtest_dir, '*.cpp')):
    with open(src_path, "rt") as fin:
        srcs.append(fin.read())


mod_srcs = []
for src in srcs:
    mod_srcs.append(do_what_is_possible(src))


out_path = os.path.join('Core', 'test', 'port.cpp')

with open(out_path, "wt") as fout:
    for mod_src in mod_srcs:
        fout.write(mod_src)
        fout.write("\n"*3)
