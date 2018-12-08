from random import randint, choice, seed
import random
import sys

import numpy as np
import matplotlib.pyplot as plt
from matplotlib.patches import Polygon

from subprocess import Popen, PIPE

angle = lambda v1, v2: np.arctan2(np.cross(v1, v2), np.dot(v1, v2))


def segIntersectsLine(seg1, seg2, l1, l2):
    return np.cross(seg1 - l1, l2 - l1) * np.cross(seg2 - l1, l2 - l1) <= 0


def ptOnSeg(p, a, b):
    return np.dot(b - a, p - a) >= 0 and np.dot(a - b, p - b) >= 0 and np.cross(a - b, p - b) == 0


def segsIntersects(a1, b1, a2, b2):
    ab1 = b1 - a1
    ab2 = b2 - a2
    if np.cross(ab1, ab2) != 0:
        return segIntersectsLine(a1, b1, a2, b2) and segIntersectsLine(a2, b2, a1, b1)
    if np.cross(ab1, a2 - a1) != 0:
        return False

    xs1, ys1 = [a1[0], b1[0]], [a1[1], b1[1]]
    xs2, ys2 = [a2[0], b2[0]], [a2[1], b2[1]]

    for coords in (xs1, ys1, xs2, ys2):
        coords.sort()

    xBeg = max(xs1[0], xs2[0])
    xEnd = min(xs1[1], xs2[1])

    yBeg = max(ys1[0], ys2[0])
    yEnd = min(ys1[1], ys2[1])

    return xBeg <= xEnd and yBeg <= yEnd


def good(poly):
    dotsCycle = poly + poly
    n = len(poly)
    for i2 in range(n):
        for i1 in range(i2):
            a1, b1 = dotsCycle[i1:i1 + 2]
            a2, b2 = dotsCycle[i2:i2 + 2]
            if (a1 == b1).all() or (a2 == b2).all():
                return False
            if segsIntersects(a1, b1, a2, b2):
                if i1 + 1 == i2 or (i1 == 0 and i2 == n - 1):
                    if i1 == 0 and i2 == n - 1:
                        (a1, b1), (a2, b2) = (a2, b2), (a1, b1)
                    assert (b1 == a2).all()
                    a, m, b = a1, b1, b2
                    if np.dot(m - b, m - a) > 0 and np.cross(m - b, m - a) == 0:
                        return False
                else:
                    return False
    return True


def randPoly(n):
    rp = lambda: np.array([randint(minX, maxX), randint(minY, maxY)])
    pts = [rp(), rp()]

    iters = 0
    while len(pts) < n:
        iters += 1
        p = rp()
        valid = []
        prob = []

        cands = list(range(len(pts)))
        cands.sort(key=lambda i: np.dot(p - pts[i], p - pts[i]))

        for i in cands:
            if valid:
                break
            new = pts[:i] + [p] + pts[i:]
            if good(new):
                valid.append(i)
                p1, p2 = pts[i], pts[(i + 1) % len(pts)]
                a = abs(angle(p - p1, p2 - p1))
                b = abs(angle(p - p2, p1 - p2))
                prob.append(1 / max(a, b))

        if not valid:
            if iters > 100:
                pts = [rp(), rp()]
                iters = 0
            continue

        iters = 0
        i = random.choices(valid, weights=prob)[0]
        pts = pts[:i] + [p] + pts[i:]
    return pts


def checkPoint(p, poly):
    an = 0
    for a, b in zip(poly, poly[1:] + poly[:1]):
        if ptOnSeg(p, a, b):
            return "BORDER"
        an += angle(a - p, b - p)

    an = abs(an)

    if an < 1e-3:
        return "OUTSIDE"
    else:
        assert abs(np.pi * 2 - an) < 1e-3
        return "INSIDE"


def makeQueries(poly):
    queries = []
    for x in range(minX - w, maxX + 1 + w):
        for y in range(minY - w, maxY + 1 + w):
            p = np.array([x, y])
            queries.append(p)
    return queries


def run(path, poly, queries):
    fmt = lambda p: "%s %s" % tuple(p)
    list2str = lambda l: "{}\n{}\n".format(len(l), "\n".join(map(fmt, l)))

    with Popen(path, encoding="ascii", stdin=PIPE, stdout=PIPE) as proc:
        inp = "1\n" + list2str(poly) + list2str(queries)
        with open("input.txt", "wt") as f:
            f.write(inp)
        out, _ = proc.communicate(inp)
    return out.split()


def plotPoly(poly):
    plt.gca().add_artist(Polygon(poly, facecolor="none", edgecolor="black"))
    plt.scatter(*zip(*poly), marker="x", c="black", zorder=1000)


def test(execPath, poly, queries):
    ans = [checkPoint(pt, poly) for pt in queries]
    got = run(execPath, poly, queries)

    if ans == got:
        return True

    bad = [i for i, (ans_, got_) in enumerate(zip(ans, got)) if ans_ != got_]

    plt.gca().add_artist(Polygon(poly, facecolor="none", edgecolor="black"))

    pos2color = {"INSIDE": "green", "OUTSIDE": "red", "BORDER": "blue"}

    plotPoly(poly)

    for i in bad:
        pt = queries[i]
        plt.scatter(pt[0], pt[1], c=pos2color[got[i]])

    plt.gca().set_aspect("equal")
    # plt.gca().set_xlim((minX - w - 1, maxX + w + 1))
    # plt.gca().set_ylim((minY - w - 1, maxY + w + 1))
    plt.show()

    return False


seed(0)

minX, maxX = 0, 8
minY, maxY = 0, 8
w = 1

if len(sys.argv) < 2:
    print("Usage: %s <path_to_executable>" % sys.argv[0])
    sys.exit(1)

path = sys.argv[1]

if 1:
    total = 1000
    log = lambda it: print("{:4}/{:4}".format(it, total))
    for it in range(total):
        if it % 10 == 0:
            log(it)
        n = random.randint(3, 16)
        poly = randPoly(n)
        queries = makeQueries(poly)

        if not test(path, poly, queries):
            break
    else:
        log(total)

else:
    fix = lambda pts: list(map(np.array, pts))

    poly = fix([
        (0, 0),
        (2, 0),
        (2, 2),
        (0, 2),
    ])

    queries = fix([
        (1, 1),
        (0, 0),
        (0, 1),
        (0, 3),
    ])

    test(poly, queries)
