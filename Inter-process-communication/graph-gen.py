#!/usr/bin/env python
import re
import sys


class Vertex(object):
    def __init__(self, x, y):
        self.x = float(x)
        self.y = float(y)

    def __repr__(self):
        return '({0:.4f},{1:.4f})'.format(self.x, self.y)

    # def __hash__(self):
    #     return hash((self.x, self.y))

    def __eq__(self, other):
        if not isinstance(other, Vertex):
            return NotImplemented
        return (self.x, self.y) == (other.x, other.y)

    def __ne__(self, other):
        # Not strictly necessary, but to avoid having both x==y and x!=y
        # True at the same time
        return not (self == other)


class LineSegment(object):
    def __init__(self, vertex1, vertex2):
        self.v1 = vertex1
        self.v2 = vertex2

    def __repr__(self):
        return '(' + repr(self.v1) + ','+ repr(self.v2) + ')'

    def __eq__(self, other):
        if not isinstance(other, LineSegment):
            return NotImplemented
        return (self.v1, self.v2) == (other.v1, other.v2)


def check_input(commands, street_dict, output_vertices_dict, output_edges_list, intersection_list):
    if len(re.findall(r'^[g]$', commands)) == 1:
        # print(re.findall(r'^[g]$', commands))
        graph(street_dict, output_vertices_dict, output_edges_list, intersection_list)
        return
    if len(re.findall(
            r'[a](?:\s)+"(?:\s)*[A-Za-z0-9]+(?:(?:\s)+[A-Za-z0-9]+)*"(?:\s)+\({1}(?:\-)?[0-9]+(?:\.[0-9]+)?,(?:\s)*(?:\-)?[0-9]+(?:\.[0-9]+)?\){1}(?:(?:\s)*\({1}(?:\-)?[0-9]+(?:\.[0-9]+)?,(?:\-)?[0-9]+(?:\.[0-9]+)?\){1})+',
            commands)) == 1:
        command_string = re.findall(
            r'[a](?:\s)+"(?:\s)*[A-Za-z0-9]+(?:(?:\s)+[A-Za-z0-9]+)*"(?:\s)+\({1}(?:\-)?[0-9]+(?:\.[0-9]+)?,(?:\s)*(?:\-)?[0-9]+(?:\.[0-9]+)?\){1}(?:(?:\s)*\({1}(?:\-)?[0-9]+(?:\.[0-9]+)?,(?:\-)?[0-9]+(?:\.[0-9]+)?\){1})+',
            commands)[0]
        if len(command_string) == len(commands):
            temp = read_input(command_string)
            add_street(street_dict, temp[0], temp[1])
            return
    if len(re.findall(
            r'[c](?:\s)+"(?:\s)*[A-Za-z0-9]+(?:(?:\s)+[A-Za-z0-9]+)*"(?:\s)+\({1}(?:\-)?[0-9]+(?:\.[0-9]+)?,(?:\s)*(?:\-)?[0-9]+(?:\.[0-9]+)?\){1}(?:(?:\s)*\({1}(?:\-)?[0-9]+(?:\.[0-9]+)?,(?:\-)?[0-9]+(?:\.[0-9]+)?\){1})+',
            commands)) == 1:
        command_string = re.findall(
            r'[c](?:\s)+"(?:\s)*[A-Za-z0-9]+(?:(?:\s)+[A-Za-z0-9]+)*"(?:\s)+\({1}(?:\-)?[0-9]+(?:\.[0-9]+)?,(?:\s)*(?:\-)?[0-9]+(?:\.[0-9]+)?\){1}(?:(?:\s)*\({1}(?:\-)?[0-9]+(?:\.[0-9]+)?,(?:\-)?[0-9]+(?:\.[0-9]+)?\){1})+',
            commands)[0]
        if len(command_string) == len(commands):
            temp = read_input(command_string)
            change_street(street_dict, temp[0], temp[1])
            return
    if len(re.findall(r'[r](?:\s)+"(?:\s)*[A-Za-z0-9]+(?:(?:\s)+[A-Za-z0-9]+)*"', commands)) == 1:
        command_string = re.findall(r'[r](?:\s)+"(?:\s)*[A-Za-z0-9]+(?:(?:\s)+[A-Za-z0-9]+)*"', commands)[0]
        if len(command_string) == len(commands):
            street_name = re.findall(r'"([^"]*)"',command_string)[0]
            street_name = street_name.lower()
            remove_street(street_dict, street_name)
            return
    sys.stderr.write("Error: The command is not in the right format. \n")
    # check_input(commands, street_dict, output_vertices_dict, output_edges_list, intersection_list)


def read_input(command_string):
    # street_name = re.findall(r'"[A-Za-z0-9]+(?:\s[A-Za-z0-9]+)*"', command_string)[0]
    street_name = re.findall(r'"([^"]*)"', command_string)[0]
    street_name = street_name.lower()
    vertices_str_list = re.findall(r'\((?:\-)?[0-9]+(?:\.[0-9]+)?,(?:\-)?[0-9]+(?:\.[0-9]+)?\)', command_string)
    vertices_tuple_list = []
    for i in vertices_str_list:
        vertex_coor = re.findall(r'(?:\-)?[0-9]+(?:\.[0-9]+)?', i)
        vertices_tuple_list.append(Vertex(vertex_coor[0], vertex_coor[1]))
    # print(vertices_tuple_list)
    line_segment_list = []
    for i in range(len(vertices_tuple_list) - 1):
        line_segment_list.append(LineSegment(vertices_tuple_list[i], vertices_tuple_list[i + 1]))
    # print(line_segment_list)
    output = [street_name,line_segment_list]
    return output


def add_street(street_dict, street_name, line_segment_list):
    if street_name in street_dict:
        sys.stderr.write("Error: 'a' specified for a street that already exist. ")
        # print("Error: 'a' specified for a street that already exist. ")
    else:
        street_dict[street_name] = line_segment_list
    # print(street_dict)


def change_street(street_dict, street_name, line_segment_list):
    if street_name in street_dict:
        street_dict[street_name] = line_segment_list
    else:
        sys.stderr.write("Error: 'c' specified for a street that does not exist. ")
        # print("Error: 'c' specified for a street that does not exist. ")


def remove_street(street_dict, street_name):
    if street_name in street_dict:
        del street_dict[street_name]
    else:
        sys.stderr.write("Error: 'r' specified for a street that does not exist. ")
        # print("Error: 'r' specified for a street that does not exist. ")


def graph(street_dict, output_vertices_dict, output_edges_list, intersection_list):
    output_vertices_dict.clear()
    del output_edges_list[:]
    del intersection_list[:]
    street_name_list = street_dict.keys()
    for index in range(len(street_name_list)):
        street_name_1 = street_name_list[index]
        for street_name_2 in street_name_list[index+1:]:
            # print(street_name_1, street_name_2)
            for line_segment_1 in street_dict[street_name_1]:
                for line_segment_2 in street_dict[street_name_2]:
                    # print(line_segment_1,line_segment_2)
                    generate_intersection_vertex_edge(line_segment_1, line_segment_2, output_vertices_dict, output_edges_list, intersection_list)
    split_edges(output_edges_list, intersection_list)
    output_format(output_vertices_dict, output_edges_list)


# def output_format(vertices_dict, edges_list):
#     # print(vertices_dict)
#     sorted_vertices_list = sorted(vertices_dict.items(), key=lambda item: item[1])
#     sys.stdout.write("V = {\n")
#     for item in sorted_vertices_list:
#         sys.stdout.write("  {0}:  ({1:.2f},{2:.2f})\n".format(item[1], item[0].x, item[0].y))
#     sys.stdout.write("}\n")
#
#     sys.stdout.write("E = {\n")
#     if len(edges_list)>0:
#         for edge in edges_list[0:len(edges_list)-1]:
#             sys.stdout.write("  <{0},{1}>,\n".format(vertices_dict[edge.v1], vertices_dict[edge.v2]))
#         sys.stdout.write("  <{0},{1}>\n".format(vertices_dict[edges_list[len(edges_list)-1].v1], vertices_dict[edges_list[len(edges_list)-1].v2]))
#     sys.stdout.write("}\n")


def output_format(vertices_dict, edges_list):
    # print(vertices_dict)
    # print(edges_list)
    sorted_vertices_list = sorted(vertices_dict.items(), key=lambda item: item[1])
    vertices_list = []
    for i in sorted_vertices_list:
        vertices_list.append(i[0])
    # print(vertices_list)
    sys.stdout.write("V {0}\n".format(len(sorted_vertices_list)))
    sys.stdout.flush()
    index1 = 0
    index2 = 0
    sys.stdout.write("E {")
    if len(edges_list)>0:
        for edge in edges_list[0:len(edges_list)-1]:
            for i in vertices_list:
                if are_vertex_the_same(i, edge.v1):
                    index1 = vertices_list.index(i)
            for j in vertices_list:
                if are_vertex_the_same(j, edge.v2):
                    index2 = vertices_list.index(j)
            sys.stdout.write("<{0},{1}>,".format(index1, index2))
                # sys.stdout.flush()
        for i in vertices_list:
            if are_vertex_the_same(i, edges_list[len(edges_list)-1].v1):
                index3 = vertices_list.index(i)
        for j in vertices_list:
            if are_vertex_the_same(j, edges_list[len(edges_list)-1].v2):
                index4 = vertices_list.index(j)
        sys.stdout.write("<{0},{1}>".format(index3, index4))
        # sys.stdout.flush()
    sys.stdout.write("}\n")
    sys.stdout.flush()



def line_function(line_segment):
    #line_segment an instance of LineSegment in the format of ((1.0, 4.0), (5.0, 8.0)).
    #when x1 != x2 return function in the format of y = slope*x + intercept
    #when x1 = x2, return 0
    line_function_dict = {}
    x1 = line_segment.v1.x
    y1 = line_segment.v1.y
    x2 = line_segment.v2.x
    y2 = line_segment.v2.y
    if x1 != x2:
        line_function_dict["slope"] = (y2 - y1) / (x2 - x1)
        line_function_dict["intercept"] = (x2 * y1 - x1 * y2) / (x2 - x1)
        return line_function_dict
    if x1 == x2:
        return 0


def is_intersection_in_line_range(line_segment, intersection):
    x1 = line_segment.v1.x
    x2 = line_segment.v2.x
    y1 = line_segment.v1.y
    y2 = line_segment.v2.y
    x_list = [x1, x2]
    x_list.sort()
    y_list = [y1, y2]
    y_list.sort()
    if round(x_list[0],4) <= round(intersection.x,4) <= round(x_list[1],4) and round(y_list[0],4) <= round(intersection.y,4) <= round(y_list[1],4):
        return True
    else:
        return False


def is_point_on_line(line, point):
    if line_function(line) == 0:
        if point.x == line.v1.x and is_intersection_in_line_range(line, point):
            return True
    if type(line_function(line)) is dict:
        if round(point.y,4) == round(point.x * line_function(line)["slope"] + line_function(line)["intercept"],4):
            if is_intersection_in_line_range(line, point):
                return True


def check_head_to_tail_line(line1, line2):
    flag = 0
    p1 = line1.v1
    p2 = line1.v2
    p3 = line2.v1
    p4 = line2.v2
    if type(line_function(line1)) is dict and type(line_function(line2)) is dict:
        k1 = line_function(line1)["slope"]
        k2 = line_function(line2)["slope"]
        b1 = line_function(line1)["intercept"]
        b2 = line_function(line1)["intercept"]
        xlist1 = [p1.x, p2.x]
        xlist2 = [p3.x, p4.x]
        xlist1.sort()
        xlist2.sort()
        if k1 == k2 and b1 == b2:
            if p3.x <= xlist1[0] or p3.x >= xlist1[1]:
                if p4.x <= xlist1[0] or p4.x >= xlist1[1]:
                    if p1.x <= xlist2[0] or p1.x >= xlist2[1]:
                        if p2.x <= xlist2[0] or p2.x >= xlist2[1]:
                            if LineSegment(p1, p2) != LineSegment(p3, p4) and LineSegment(p1, p2) != LineSegment(p4, p3):
                                for p_a in [p3,p4]:
                                    for p_b in [p1, p2]:
                                        if p_a == p_b:
                                            flag = 1
                                            intersection = p_a
    if line_function(line1) == 0 and line_function(line2) == 0:
        ylist1 = [p1.y, p2.y]
        ylist2 = [p3.y, p4.y]
        ylist1.sort()
        ylist2.sort()
        if p1.x == p3.x:
            if p3.y <= ylist1[0] or p3.y >= ylist1[1]:
                if p4.y <= ylist1[0] or p4.y >= ylist1[1]:
                    if p1.y <= ylist2[0] or p1.y >= ylist2[1]:
                        if p2.y <= ylist2[0] or p2.y >= ylist2[1]:
                            if LineSegment(p1, p2) != LineSegment(p3, p4) and LineSegment(p1, p2) != LineSegment(p4, p3):
                                for p_a in [p3,p4]:
                                    for p_b in [p1, p2]:
                                        if p_a == p_b:
                                            flag = 1
                                            intersection = p_a
    if flag == 1:
        return [True, intersection]
    else:
        return [False]



def append_to_vertices_edge_list(output_vertices_dict, output_edges_list, vertex_list):
    for vertex in vertex_list:
        # print(output_vertices_dict.keys())
        flag = 1
        for exist_vertex in output_vertices_dict.keys():
            if are_vertex_the_same(vertex, exist_vertex):
                flag = 0
        if flag == 1:
            # print("add 1")
            output_vertices_dict[vertex] = len(output_vertices_dict.keys())
            # output_vertices_dict[vertex] = len(output_vertices_dict.keys()) + 1
    intersection = vertex_list[0]
    for vertex2 in vertex_list[1:]:
        flag2 = 1
        if are_vertex_the_same(intersection, vertex2):
            flag2 = 0
        for exist_edge in output_edges_list:
            if are_lines_the_same(LineSegment(intersection, vertex2), exist_edge):
                flag2 = 0
        if flag2 == 1:
            # for vertex3 in output_vertices_dict
            output_edges_list.append(LineSegment(intersection, vertex2))


def append_to_point_list(point, point_list):
    flag = 1
    for exist_point in point_list:
        if are_vertex_the_same(exist_point,point):
            flag = 0
    if flag == 1:
        point_list.append(point)


def are_vertex_the_same(vertex1, vertex2):
    if round(vertex1.x, 4) == round(vertex2.x, 4) and round(vertex1.y, 4) == round(vertex2.y, 4):
        return True
    else:
        return False


# def are_lines_the_same(line1, line2):
#     if round(line1.v1.x, 4) == round(line2.v1.x, 4) and round(line1.v1.y, 4) == round(line2.v1.y, 4) and round(line1.v2.x, 4) == round(line2.v2.x, 4) and round(line1.v2.y,4) == round(line2.v2.y,4):
#         return True
#     elif round(line1.v1.x, 4) == round(line2.v2.x, 4) and round(line1.v1.y, 4) == round(line2.v2.y, 4) and round(line1.v2.x, 4) == round(line2.v1.x, 4) and round(line1.v2.y,4) == round(line2.v1.y,4):
#         return True
#     else:
#         return False


def are_lines_the_same(line1, line2):
    if line1.v1 == line2.v1 and line1.v2 == line2.v2:
        return True
    elif line1.v1 == line2.v2 and line1.v2 == line2.v1:
        return True
    else:
        return False


def append_to_edge_list(output_edges_list, line_seg):
    flag = 1
    for edge in output_edges_list:
        if are_lines_the_same(line_seg, edge):
            flag = 0
    if flag == 1:
        output_edges_list.append(line_seg)


def generate_intersection_vertex_edge(line_segment_1, line_segment_2, output_vertices_dict, output_edges_list, intersection_list):
    x1 = line_segment_1.v1.x
    x3 = line_segment_2.v1.x
    line_segment_1_func = line_function(line_segment_1)
    line_segment_2_func = line_function(line_segment_2)
    if line_segment_1_func == 0:
        if type(line_segment_2_func) is dict:
            k2 = line_segment_2_func["slope"]
            b2 = line_segment_2_func["intercept"]
            intersection_x = x1
            intersection_y = k2 * x1 + b2
            if is_intersection_in_line_range(line_segment_1, Vertex(intersection_x, intersection_y)) and is_intersection_in_line_range(line_segment_2, Vertex(intersection_x, intersection_y)):
                intersection = Vertex(intersection_x, intersection_y)
                append_to_vertices_edge_list(output_vertices_dict, output_edges_list, [intersection, line_segment_1.v1, line_segment_1.v2, line_segment_2.v1, line_segment_2.v2])
                append_to_point_list(intersection, intersection_list)
    if type(line_segment_1_func) is dict:
        if line_segment_2_func == 0:
            k1 = line_segment_1_func["slope"]
            b1 = line_segment_1_func["intercept"]
            intersection_x = x3
            intersection_y = k1 * x3 + b1
            if is_intersection_in_line_range(line_segment_1, Vertex(intersection_x, intersection_y)) and is_intersection_in_line_range(line_segment_2, Vertex(intersection_x, intersection_y)):
                intersection = Vertex(intersection_x, intersection_y)
                append_to_vertices_edge_list(output_vertices_dict, output_edges_list, [intersection, line_segment_1.v1, line_segment_1.v2, line_segment_2.v1, line_segment_2.v2])
                append_to_point_list(intersection, intersection_list)
    if type(line_segment_1_func) is dict:
            if type(line_segment_2_func) is dict:
                k1 = line_segment_1_func["slope"]
                b1 = line_segment_1_func["intercept"]
                k2 = line_segment_2_func["slope"]
                b2 = line_segment_2_func["intercept"]
                # if k1 == k2:
                #     continue
                if k1 != k2:
                    intersection_x = (b2 - b1) / (k1 - k2)
                    intersection_y = (k1*b2 - k2*b1) / (k1 - k2)
                    if is_intersection_in_line_range(line_segment_1,
                                               Vertex(intersection_x, intersection_y)) and is_intersection_in_line_range(
                            line_segment_2, Vertex(intersection_x, intersection_y)):
                        intersection = Vertex(intersection_x, intersection_y)
                        append_to_vertices_edge_list(output_vertices_dict, output_edges_list,
                                                     [intersection, line_segment_1.v1, line_segment_1.v2, line_segment_2.v1,
                                                      line_segment_2.v2])
                        append_to_point_list(intersection, intersection_list)
    if check_head_to_tail_line(line_segment_1, line_segment_2)[0]:
        intersection = check_head_to_tail_line(line_segment_1, line_segment_2)[1]
        append_to_vertices_edge_list(output_vertices_dict, output_edges_list,
                                     [intersection, line_segment_1.v1, line_segment_1.v2, line_segment_2.v1,
                                      line_segment_2.v2])
        append_to_point_list(intersection, intersection_list)


def split_edges(output_edges_list, intersection_list):
    remove_line_list = []
    append_line_list = []
    for line in output_edges_list:
        temp_point_list = []
        for point in intersection_list:
            if is_point_on_line(line, point):
                temp_point_list.append(point)
        if len(temp_point_list) > 1:
            if line_function(line) == 0:
                temp_point_list.sort(key=lambda p: p.y)
            if type(line_function(line)) is dict:
                temp_point_list.sort(key=lambda p: p.x)
            remove_line_list.append(line)
            for i in range(len(temp_point_list)-1):
                append_line_list.append(LineSegment(temp_point_list[i], temp_point_list[i+1]))
    for line in remove_line_list:
        for line2 in output_edges_list:
            if are_lines_the_same(line, line2):
                output_edges_list.remove(line2)
    for line in append_line_list:
        append_to_edge_list(output_edges_list, line)


def main():
    street_dict = {}
    intersection_list = []
    output_vertices_dict = {}
    output_edges_list = []
    while True:
        try:
            # sys.stdout.write("Please type your commands (return to quit): \n")
            commands = sys.stdin.readline()
            if commands =='':
                break
            commands = commands.strip('\n')
        except EOFError:
            break
        check_input(commands, street_dict, output_vertices_dict, output_edges_list, intersection_list)
    # return exit code 0 on successful termination
    sys.exit(0)


if __name__ == '__main__':
    main()
