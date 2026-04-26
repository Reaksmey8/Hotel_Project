#pragma once
// Custom Table class - works like tabulate but no locale crash
#include <string>
#include <vector>
#include <iostream>
#include <iomanip>
#include <sstream>
using namespace std;

namespace tabulate
{

    // Font styles (same as tabulate)
    enum class FontStyle
    {
        bold,
        italic,
        underline
    };
    enum class FontAlign
    {
        left,
        center,
        right
    };
    enum class Color
    {
        red,
        green,
        yellow,
        blue,
        cyan,
        white,
        none
    };

    // ── Format class ─────────────────────────────────────────────
    class Format
    {
    public:
        FontAlign align = FontAlign::left;
        bool bold = false;
        Color color = Color::none;

        Format &font_style(vector<FontStyle> styles)
        {
            for (auto &s : styles)
                if (s == FontStyle::bold)
                    bold = true;
            return *this;
        }
        Format &font_align(vector<FontAlign> aligns)
        {
            if (!aligns.empty())
                align = aligns[0];
            return *this;
        }
        Format &font_align(FontAlign a)
        {
            align = a;
            return *this;
        }
        Format &font_color(Color c)
        {
            color = c;
            return *this;
        }
        Format &font_color(vector<Color> c)
        {
            if (!c.empty())
                color = c[0];
            return *this;
        }
        Format &width(int) { return *this; }
        Format &padding(int) { return *this; }
        Format &border(const string &) { return *this; }
        Format &border_color(Color) { return *this; }
    };

    // ── Cell class ────────────────────────────────────────────────
    class Cell
    {
    public:
        string text;
        Format fmt;
        Format &format() { return fmt; }
        const string &get_text() const { return text; }
    };

    // ── Row class ────────────────────────────────────────────────
    class Row
    {
    public:
        vector<Cell> cells;
        Format fmt;

        Cell &operator[](size_t i) { return cells[i]; }
        Format &format() { return fmt; }

        size_t size() const { return cells.size(); }
    };

    // ── Table class ───────────────────────────────────────────────
    class Table
    {
    private:
        vector<Row> rows;

        string colorCode(Color c) const
        {
            switch (c)
            {
            case Color::red:
                return "\033[1;31m";
            case Color::green:
                return "\033[1;32m";
            case Color::yellow:
                return "\033[1;33m";
            case Color::blue:
                return "\033[1;34m";
            case Color::cyan:
                return "\033[1;36m";
            case Color::white:
                return "\033[1;37m";
            default:
                return "";
            }
        }

        string applyFormat(const string &text, const Format &fmt,
                           int width) const
        {
            string result = text;
            // truncate if too long
            if ((int)result.size() > width)
                result = result.substr(0, width);

            // alignment padding
            int spaces = width - (int)result.size();
            if (fmt.align == FontAlign::center)
            {
                int left = spaces / 2;
                int right = spaces - left;
                result = string(left, ' ') + result + string(right, ' ');
            }
            else if (fmt.align == FontAlign::right)
            {
                result = string(spaces, ' ') + result;
            }
            else
            {
                result = result + string(spaces, ' ');
            }

            // color + bold
            string prefix = "";
            string suffix = "";
            if (fmt.color != Color::none)
            {
                prefix += colorCode(fmt.color);
                suffix = "\033[0m";
            }
            if (fmt.bold && prefix.empty())
            {
                prefix = "\033[1m";
                suffix = "\033[0m";
            }
            return prefix + result + suffix;
        }

        // compute max width for each column
        vector<int> colWidths() const
        {
            if (rows.empty())
                return {};
            size_t cols = 0;
            for (auto &r : rows)
                cols = max(cols, r.cells.size());
            vector<int> widths(cols, 0);
            for (auto &row : rows)
            {
                for (size_t c = 0; c < row.cells.size(); c++)
                {
                    widths[c] = max(widths[c], (int)row.cells[c].text.size());
                }
            }
            // minimum width
            for (auto &w : widths)
                w = max(w, 4);
            return widths;
        }

        void printSeparator(const vector<int> &widths) const
        {
            cout << "  +";
            for (size_t i = 0; i < widths.size(); i++)
            {
                for (int j = 0; j < widths[i] + 2; j++)
                    cout << "-";
                cout << "+";
            }
            cout << "\n";
        }

    public:
        void add_row(vector<string> texts)
        {
            Row row;
            for (auto &t : texts)
            {
                Cell c;
                c.text = t;
                row.cells.push_back(c);
            }
            rows.push_back(row);
        }

        Row &operator[](size_t i) { return rows[i]; }

        // Print the table
        friend ostream &operator<<(ostream &os, const Table &t)
        {
            if (t.rows.empty())
                return os;
            auto widths = t.colWidths();

            // top border
            os << "  +";
            for (size_t i = 0; i < widths.size(); i++)
            {
                for (int j = 0; j < widths[i] + 2; j++)
                    os << "-";
                os << "+";
            }
            os << "\n";

            for (size_t r = 0; r < t.rows.size(); r++)
            {
                const Row &row = t.rows[r];
                os << "  |";
                for (size_t c = 0; c < row.cells.size(); c++)
                {
                    const Cell &cell = row.cells[c];
                    // merge row format and cell format
                    Format fmt = cell.fmt;
                    if (t.rows[r].fmt.bold)
                        fmt.bold = true;
                    if (t.rows[r].fmt.color != Color::none)
                        fmt.color = t.rows[r].fmt.color;
                    if (t.rows[r].fmt.align != FontAlign::left)
                        fmt.align = t.rows[r].fmt.align;

                    int w = (c < widths.size()) ? widths[c] : 10;
                    os << " " << t.applyFormat(cell.text, fmt, w) << " |";
                }
                os << "\n";

                // separator after header row
                if (r == 0)
                {
                    os << "  +";
                    for (size_t i = 0; i < widths.size(); i++)
                    {
                        for (int j = 0; j < widths[i] + 2; j++)
                            os << "=";
                        os << "+";
                    }
                    os << "\n";
                }
            }

            // bottom border
            os << "  +";
            for (size_t i = 0; i < widths.size(); i++)
            {
                for (int j = 0; j < widths[i] + 2; j++)
                    os << "-";
                os << "+";
            }
            os << "\n";
            return os;
        }
    };

} // namespace tabulate