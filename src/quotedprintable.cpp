/*
  Copyright (c) 2011-2012 - Tőkés Attila
  Copyright (C) 2015 Daniel Nicoletti <dantti12@gmail.com>

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  See the LICENSE file for more details.
*/

#include "quotedprintable.h"

using namespace SimpleMail;

QByteArray QuotedPrintable::encode(const QByteArray &input, int *printable, int *encoded)
{
    QByteArray output;

    char byte;
    static const char hex[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};

    for (int i = 0; i < input.length() ; ++i) {
        byte = input[i];

        if (byte == ' ') {
            output.append('_');
            if (printable) {
                ++(*printable);
            }
        } else if (byte == ':') {
            output.append("=3A");
            if (printable) {
                ++(*printable);
            }
        } else if ((byte >= 33) && (byte <= 126) && (byte != 61)) {
            output.append(byte);
            if (printable) {
                ++(*printable);
            }
        } else {
            output.append('=');
            output.append(hex[((byte >> 4) & 0x0F)]);
            output.append(hex[(byte & 0x0F)]);
            if (encoded) {
                ++(*encoded);
            }
        }
    }

    return output;
}

QByteArray QuotedPrintable::decode(const QByteArray &input)
{
    //                           0  1  2  3  4  5  6  7  8  9  :  ;  <  =  >  ?  @  A   B   C   D   E   F
    static const int hexVal[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 0, 0, 0, 0, 0, 0, 0, 10, 11, 12, 13, 14, 15};

    QByteArray output;

    int len = input.length();
    for (int i = 0; i < len-2; ++i) {
        if (input.at(i) == '=') {
            int x = input.at(i+1) - '0';
            int y = input.at(i+2) - '0';
            if (x >= 0 && y >= 0 && x < 23 && y < 23) {
                output.append(char((hexVal[x] << 4) + hexVal[y]));
            }
            else {
                output.append('=').append(char(x + '0')).append(char(y + '0'));
            }

            i += 2;
        } else {
            output.append(input.at(i));
        }
    }

    return output;
}
