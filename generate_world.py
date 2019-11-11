import enum
import string
import sys

class InFlag(enum.Flag):
    no = 0
    top = enum.auto()
    mid = enum.auto()
    bot = enum.auto()

def printe(*args, **kwargs):
    print(*args, file=sys.stderr, **kwargs)

template = """
#define WORLD_SIZE {world_size}
char max_map_depth = {world_depth};
char nodes_x[]   = {node_xs};
char nodes_y[]   = {node_ys};
char nodes_out[] = {node_out};
char nodes_in[]  = {node_in};
char num_of_links = {num_of_links};
char links[] = {links};
content_type nodes_content[] = {node_content};
char node_args[] = {node_args};
"""

skip_node = '='
no_node = 'n'
# just node [0-9]

up = "Г".lower()
straight = "-".lower()
down = "L".lower()
up_and_straight = "P".lower()
down_and_straight = "b".lower()
down_and_up = "K".lower()
all_ = "E".lower()
alt_all = "Е".lower() # cyr e

char2inflag = {
    ' ': InFlag.no,
    up: InFlag.top,
    straight: InFlag.mid,
    down: InFlag.bot,
    up_and_straight: (InFlag.top | InFlag.mid),
    down_and_straight: (InFlag.bot | InFlag.mid),
    down_and_up: (InFlag.bot | InFlag.top),
    all_: (InFlag.bot | InFlag.top | InFlag.mid),
    alt_all: (InFlag.bot | InFlag.top | InFlag.mid),
}

(
    JUST,
    NO, 
    SKIP,
) = (
    "JUST",
    "NO_NODE",
    "SKIP_NODE"
)

#    02 01 00
#      /| \
#    09 06 03
#      \| /|
#    10 07 04
#       | / 
#    11 08 05

world_0 = """
n   0 b 0 L n
0 E = - 0 - 0
n   0 Г n   n
"""








y2inflag = {
    0: InFlag.top,
    1: InFlag.mid,
    2: InFlag.bot,
}

class Node:

    def __init__(self, node_type, arg=0, inflag=InFlag.no, isout=False):
        self.node_type = node_type
        self.arg = arg
        self.inflag = inflag
        self.isout = isout
        self.links = []
    def __repr__(self):
        return f"Node {self.node_type}, {self.arg}, {self.inflag}, {self.isout}"

def generate_world_struct(world: str) -> (dict, list):

    # x,y -> type, arg, in, out
    world_d = {
        (0, 0): Node(NO, 0, InFlag.no, False),
        (0, 1): Node(JUST, 0, InFlag.no, False),
        (0, 2): Node(NO, 0, InFlag.no, False),
    }
    all_links = []
    world = world.strip()
    for y, world_line in enumerate(world.splitlines()):
        # world_chars = world_line.split()[2::]   # first col always same
        links = world_line[2::4]
        worlds = world_line[4::4]

        for x, (world_type, link) in enumerate(zip(worlds, links), 1):
            world_type = world_type.lower()
            link = link.lower()

            inflag = char2inflag[link]
            if InFlag.top in inflag:
                if y == 0:
                    raise ValueError(f"Going to top in {x}")
                all_links.append((
                    (x - 1, y),
                    (x, y - 1),
                    y2inflag[y]
                ))
            if InFlag.mid in inflag:
                all_links.append((
                    (x - 1, y),
                    (x, y),
                    y2inflag[y]
                ))
            if InFlag.bot in inflag:
                # print(x, file=sys.stderr)
                if y == 2:
                    raise ValueError(f"Going to bot in {x}")
                all_links.append((
                    (x - 1, y),
                    (x, y + 1),
                    y2inflag[y]
                ))
            print((x, y), world_type, link, inflag, file=sys.stderr)

            if world_type in string.digits:
                world_d[(x, y)] = Node(JUST, int(world_type))
            elif world_type == skip_node:
                world_d[(x, y)] = Node(SKIP, 0)
            elif world_type == no_node:
                world_d[(x, y)] = Node(NO, 0)
            else:
                raise ValueError(f"Unknown char {world_type} at {(x, y)}")
    return world_d, all_links

def link_node_skip(world_d, x, y) -> (int, int):
    if world_d[(x, y)].node_type == SKIP:
        return link_node_skip(world_d, x + 1, y)
    else: 
        return x, y

def post_proc_world_struct(world_d: dict, links: list):
    # world_size = len(world_d)
    world_depth = max(world_d.keys(), key=lambda x: x[0])[0]
    # world_i = {k: i for (i, k) in enumerate(world_d.keys)}

    for from_, to, inflag in links:
        print(from_, to, inflag, file=sys.stderr)
        print(world_d[to], file=sys.stderr)
        world_d[to].inflag |= inflag
        real_c = link_node_skip(world_d, *to)
        world_d[from_].links.append(real_c)

    outs = []
    for k, v in world_d.items():
        node_type = v.node_type
        (x, y) = k
        if node_type != NO and x < world_depth:
            outs.append((x + 1, y))
    printe("outs")
    printe(outs)
    for out in outs:
        world_d[out].isout = True

def as_c_arr(arr):
    return "{" + ", ".join(map(str, arr)) + "} "

def format_world(world_d):
    world_size = len(world_d)
    world_depth = max(world_d.keys(), key=lambda x: x[0])[0]
    nodes_xs = [0 for _ in range(world_size)]
    nodes_ys = [0 for _ in range(world_size)]
    nodes_out = [False for _ in range(world_size)]
    nodes_in = [0 for _ in range(world_size)]
    nodes_content = [None for _ in range(world_size)]
    args = [0 for _ in range(world_size)]
    links = []

    world_i = {k: i for (i, k) in enumerate(world_d.keys())}
    for k, v in world_d.items():
        v: Node
        i = world_i[k]
        (x, y) = k

        nodes_xs[i] = x
        nodes_ys[i] = y
        nodes_out[i] = int(v.isout)
        nodes_in[i] = v.inflag.value
        nodes_content[i] = v.node_type
        args[i] = v.arg

        for c in v.links:
            links.extend([i, world_i[c]])

    num_of_links = len(links) // 2
    formatted_template = template.format(
        world_size=world_size,
        world_depth=world_depth,
        node_xs=as_c_arr(nodes_xs),
        node_ys=as_c_arr(nodes_ys),
        node_out=as_c_arr(nodes_out),
        node_in=as_c_arr(nodes_in),
        node_content=as_c_arr(nodes_content),
        node_args=as_c_arr(args),
        num_of_links=num_of_links,
        links=as_c_arr(links),
    )
    print(formatted_template)


if __name__ == "__main__":
    world_c = world_0
    world_d, links = generate_world_struct(world_c)
    post_proc_world_struct(world_d, links)
    format_world(world_d)
