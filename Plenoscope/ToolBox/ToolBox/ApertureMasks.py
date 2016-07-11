#!/usr/bin/env python
from __future__ import absolute_import, print_function, division
import numpy as np

def circular_mask(paxel_pos_x, paxel_pos_y, x, y, r, smear=0.0):

    r1 = r - smear
    r2 = r + smear

    if smear > 0.0:
        m = -1.0/(r2 - r1)
        b = 1.0 - m*r1

    center = np.array([x,y])
    mask = np.zeros(shape=paxel_pos_x.shape)

    for pax in range(paxel_pos_x.shape[0]):
        pax_pos = np.array([paxel_pos_x[pax], paxel_pos_y[pax]])
        dist = np.linalg.norm(center - pax_pos)
        if dist <= r1:
            mask[pax] = 1.0
        elif dist > r1 and dist < r2 and smear > 0.0:
            mask[pax] = m*dist + b
        elif dist > r2:
            mask[pax] = 0.0

    return mask