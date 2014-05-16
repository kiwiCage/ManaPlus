/*
 *  The ManaPlus Client
 *  Copyright (C) 2007-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011-2014  The ManaPlus Developers
 *
 *  This file is part of The ManaPlus Client.
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "gui/widgets/layoutarray.h"

#include "logger.h"

#include "gui/widgets/layoutcell.h"
#include "gui/widgets/layouttype.h"
#include "gui/widgets/widget.h"

#include <cassert>

#include "debug.h"

LayoutArray::LayoutArray():
    mCells(),
    mSpacing(4)
{
}

LayoutArray::~LayoutArray()
{
    std::vector <std::vector <LayoutCell *> >::iterator
        i = mCells.begin();
    const std::vector <std::vector <LayoutCell *> >::iterator
        i_end = mCells.end();
    while (i != i_end)
    {
        std::vector< LayoutCell * >::iterator j = i->begin();
        const std::vector< LayoutCell * >::iterator j_end = i->end();
        while (j != j_end)
        {
            delete *j;
            ++j;
        }
        ++i;
    }
}

LayoutCell &LayoutArray::at(const int x, const int y,
                            const int w, const int h)
{
    resizeGrid(x + w, y + h);
    LayoutCell *&cell = mCells[y][x];
    if (!cell)
        cell = new LayoutCell;
    return *cell;
}

void LayoutArray::resizeGrid(int w, const int h)
{
    const bool extW = w && w > static_cast<int>(mSizes[0].size());
    const bool extH = h && h > static_cast<int>(mSizes[1].size());

    if (!extW && !extH)
        return;

    if (extH)
    {
        mSizes[1].resize(h, LayoutType::DEF);
        mCells.resize(h);
        if (!extW)
            w = static_cast<int>(mSizes[0].size());
    }

    if (extW)
        mSizes[0].resize(w, LayoutType::DEF);

    std::vector <std::vector <LayoutCell *> >::iterator
        i = mCells.begin();
    const std::vector <std::vector <LayoutCell *> >::iterator
        i_end = mCells.end();
    while (i != i_end)
    {
        i->resize(w, nullptr);
        ++i;
    }
}

void LayoutArray::setColWidth(const int n, const int w)
{
    resizeGrid(n + 1, 0);
    mSizes[0][n] = w;
}

void LayoutArray::setRowHeight(const int n, const int h)
{
    resizeGrid(0, n + 1);
    mSizes[1][n] = h;
}

void LayoutArray::matchColWidth(const int n1, const int n2)
{
    resizeGrid(std::max(n1, n2) + 1, 0);
    const std::vector<int> widths = getSizes(0, LayoutType::DEF);
    const int s = std::max(widths[n1], widths[n2]);
    mSizes[0][n1] = s;
    mSizes[0][n2] = s;
}

void LayoutArray::extend(const int x, const int y, const int w, const int h)
{
    LayoutCell &cell = at(x, y, w, h);
    cell.mExtent[0] = w;
    cell.mExtent[1] = h;
}

LayoutCell &LayoutArray::place(Widget *const widget, const int x,
                               const int y, const int w, const int h)
{
    LayoutCell &cell = at(x, y, w, h);
    assert(cell.mType == LayoutCell::NONE);
    cell.mType = LayoutCell::WIDGET;
    cell.mWidget = widget;
    if (widget)
    {
        cell.mSize[0] = w == 1 ? widget->getWidth() : 0;
        cell.mSize[1] = h == 1 ? widget->getHeight() : 0;
    }
    else
    {
        cell.mSize[0] = 1;
        cell.mSize[1] = 1;
    }
    cell.mExtent[0] = w;
    cell.mExtent[1] = h;
    cell.mHPadding = 0;
    cell.mVPadding = 0;
    cell.mAlign[0] = LayoutCell::FILL;
    cell.mAlign[1] = LayoutCell::FILL;
    int &cs = mSizes[0][x], &rs = mSizes[1][y];
    if (cs == LayoutType::DEF && w == 1)
        cs = 0;
    if (rs == LayoutType::DEF && h == 1)
        rs = 0;
    return cell;
}

void LayoutArray::align(int &restrict pos, int &restrict size, const int dim,
                        LayoutCell const &restrict cell,
                        const int *restrict const sizes,
                        const int sizeCount) const
{
    if (dim < 0 || dim >= 2)
        return;
    int size_max = sizes[0];
    int cnt = cell.mExtent[dim];
    if (sizeCount && cell.mExtent[dim] > sizeCount)
        cnt = sizeCount;

    for (int i = 1; i < cnt; ++i)
        size_max += sizes[i] + mSpacing;
    size = std::min<int>(cell.mSize[dim], size_max);

    switch (cell.mAlign[dim])
    {
        case LayoutCell::LEFT:
            return;
        case LayoutCell::RIGHT:
            pos += size_max - size;
            return;
        case LayoutCell::CENTER:
            pos += (size_max - size) / 2;
            return;
        case LayoutCell::FILL:
            size = size_max;
            return;
        default:
            logger->log1("LayoutArray::align unknown layout");
            return;
    }
}

std::vector<int> LayoutArray::getSizes(const int dim, int upp) const
{
    if (dim < 0 || dim >= 2)
        return mSizes[1];

    const int gridW = static_cast<int>(mSizes[0].size());
    const int gridH = static_cast<int>(mSizes[1].size());
    std::vector<int> sizes = mSizes[dim];

    // Compute minimum sizes.
    for (int gridY = 0; gridY < gridH; ++gridY)
    {
        for (int gridX = 0; gridX < gridW; ++gridX)
        {
            const LayoutCell *const cell = mCells[gridY][gridX];
            if (!cell || cell->mType == LayoutCell::NONE)
                continue;

            if (cell->mExtent[dim] == 1)
            {
                const int n = (dim == 0 ? gridX : gridY);
                const int s = cell->mSize[dim] + cell->mVPadding * 2;
                if (s > sizes[n])
                    sizes[n] = s;
            }
        }
    }

    if (upp == LayoutType::DEF)
        return sizes;

    // Compute the FILL sizes.
    const int nb = static_cast<int>(sizes.size());
    int nbFill = 0;
    for (int i = 0; i < nb; ++i)
    {
        if (mSizes[dim][i] <= LayoutType::DEF)
        {
            ++nbFill;
            if (mSizes[dim][i] == LayoutType::SET ||
                sizes[i] <= LayoutType::DEF)
            {
                sizes[i] = 0;
            }
        }
        upp -= sizes[i] + mSpacing;
    }
    upp = upp + mSpacing;

    if (nbFill == 0)
        return sizes;

    for (int i = 0; i < nb; ++i)
    {
        if (mSizes[dim][i] > LayoutType::DEF)
            continue;

        const int s = upp / nbFill;
        sizes[i] += s;
        upp -= s;
        --nbFill;
    }

    return sizes;
}

int LayoutArray::getSize(const int dim) const
{
    std::vector<int> sizes = getSizes(dim, LayoutType::DEF);
    int size = 0;
    const int nb = static_cast<int>(sizes.size());
    for (int i = 0; i < nb; ++i)
    {
        if (sizes[i] > LayoutType::DEF)
            size += sizes[i];
        size += mSpacing;
    }
    return size - mSpacing;
}

void LayoutArray::reflow(const int nx, const int ny,
                         const int nw, const int nh)
{
    const int gridW = static_cast<int>(mSizes[0].size());
    const int gridH = static_cast<int>(mSizes[1].size());

    std::vector<int> widths  = getSizes(0, nw);
    std::vector<int> heights = getSizes(1, nh);

    const int szW = static_cast<int>(widths.size());
    const int szH = static_cast<int>(heights.size());
    int y = ny;
    for (int gridY = 0; gridY < gridH; ++gridY)
    {
        int x = nx;
        for (int gridX = 0; gridX < gridW; ++gridX)
        {
            LayoutCell *const cell = mCells[gridY][gridX];
            if (cell && cell->mType != LayoutCell::NONE)
            {
                int dx = x, dy = y, dw = 0, dh = 0;
                align(dx, dw, 0, *cell, &widths[gridX], szW - gridX);
                align(dy, dh, 1, *cell, &heights[gridY], szH - gridY);
                cell->reflow(dx, dy, dw, dh);
            }
            x += widths[gridX] + mSpacing;
        }
        y += heights[gridY] + mSpacing;
    }
}
