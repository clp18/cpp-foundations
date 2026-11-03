# Pitch Tracker Specification
**Version:** 1.0    
**Status:** Accepted    
**Date:** 2026-11-03

## 1. Purpose
This message represents a softball pitcher's practice sequence: a fixed 6-byte header followed by a variable-length Sequence of 3-byte pitch records.

## 2. Conventions
- **Bit order:** MSB-first
- **Byte order:** Big-endian
- **Signed values:** Two's complement
- **Padding:** Zero-filled to the next byte boundary

## 3. Message Layout
| Pitcher Header (6 bytes) | Pitch Sequence (3 bytes * Total Pitch Count) (repeating section) |
|---|---|

**Total message length:** 6 + (3 x Total Pitch Count) bytes, from 6 to 771 bytes.

### 3.1 Pitcher Header
| Name | Offset (bits) | Width (bits) | Type | Range | Description | 
|---|---|---|---|---|---|
| Year | 0 | 12 | unsigned integer | 1900 - 2046 | Year the pitcher sequence was tracked |
| Month | 12 | 4 | unsigned integer | 1 - 12 | Month the pitch sequence was tracked |
| Day | 16 | 5 | unsigned integer | 1 - 31 | Day the pitch sequence was tracked |
| Padding | 21 | 3 | unsigned integer | 0 | Padding to the byte boundary | 
| Pitcher ID | 24 | 16 | unsigned integer | 1 - 65,535 | Unique ID for each pitcher | 
| Total Pitch Count | 40 | 8 | unsigned integer | 0 - 255 | Number of pitches that follow. 0 means no Pitch Sequence. |

### 3.2 Pitch Sequence (variable length field)
The Pitch Sequence is a variable-length field made up of Total Pitch Count records. Each record is 24 bits, and offsets are relative to the start of the record.

| Name | Offset (bits) | Width (bits) | Type | Range | Description |
|---|---|---|---|---|---|
| Pitch | 0 | 4 | enumeration | 0 - 15 | See Pitch Type values below |
| Speed | 4 | 7 | unsigned integer | 1 - 120 | MPH. Range leaves room to accommodate baseball. |
| X | 11 | 4 | signed integer | -4 to 4 | Horizontal strike zone coordinate (see Section 4) |
| Y | 15 | 4 | signed integer | -4 to 4 | Vertical strike zone coordinate (see Section 4) |
| Ball or Strike | 19 | 1 | flag | 0 - 1 | Ball = 0, Strike = 1 |
| Padding | 20 | 4 | unsigned integer | 0 | Padding to the byte boundary |

**Pitch Type values:** 0 = Fastball, 1 = Changeup, 2 = Drop, 3 = Rise, 4 = Curve, 5 = Screw, 6 - 15 = Reserved

## 4. Zone Grid
The grid is viewed from the catcher's position. (0, 0) is the center of the strike zone, +X points toward the first-base side, and +Y points up.
 
| Zone | Rule |
|---|---|
| Zone | X and Y are both inclusive -2 to 2 |
| Competitive Pitch | One coordinate is -2 and 2 inclusive, and the other is 3 or -3 |
| Waste | Any other location, including either coordinate at ±4 or both at ±3 |

## 5. Validation
Reject the message if any of the following is true:
- Year, Month, Day is outside of individual field allowance.
- Year, Month, Day is not a real date. (example: 2026-02-30)
- Speed is outside of 1 - 120.
- Pitcher ID is equal to 0.
- Pitch Type is reserved (6 - 15).
- X or Y is outside -4 to 4.
- The received length is not equal to 6 + (3 × Total Pitch Count).

Padding bits are ignored on decode to allow space for expansion without rewriting all test cases.


## 6. Worked Example
Date 2026-10-24, Pitcher 42, 2 pitches:
- Pitch 1: Fastball, 62 MPH, (0, -1), Strike
- Pitch 2: Rise, 55 MPH, (3, -2), Ball (a competitive pitch)

| Section | Bytes |
|---|---|
| Header | `7E AA C0 00 2A 02` |
| Pitch 1 | `07 C1 F0` |
| Pitch 2 | `36 E7 C0` |
 
**Encoded message (12 bytes):** `7E AA C0 00 2A 02 07 C1 F0 36 E7 C0`
 