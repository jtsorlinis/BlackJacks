const HARD_TABLE = [_][10]u8{
    "HHHHHHHHHH".*, // 2
    "HHHHHHHHHH".*, // 3
    "HHHHHHHHHH".*, // 4
    "HHHHHHHHHH".*, // 5
    "HHHHHHHHHH".*, // 6
    "HHHHHHHHHH".*, // 7
    "HHHHHHHHHH".*, // 8
    "DDDDDHHHHH".*, // 9
    "DDDDDDDDHH".*, // 10
    "DDDDDDDDDH".*, // 11
    "HHSSSHHHHH".*, // 12
    "SSSSSHHHHH".*, // 13
    "SSSSSHHHHH".*, // 14
    "SSSSSHHHHH".*, // 15
    "SSSSSHHHHH".*, // 16
    "SSSSSSSSSS".*, // 17
    "SSSSSSSSSS".*, // 18
    "SSSSSSSSSS".*, // 19
    "SSSSSSSSSS".*, // 20
    "SSSSSSSSSS".*, // 21
};

const SOFT_TABLE = [_][10]u8{
    "HHHDDHHHHH".*, // 13
    "HHHDDHHHHH".*, // 14
    "HHDDDHHHHH".*, // 15
    "HHDDDHHHHH".*, // 16
    "HDDDDHHHHH".*, // 17
    "SDDDDSSHHH".*, // 18
    "SSSSSSSSSS".*, // 19
    "SSSSSSSSSS".*, // 20
    "SSSSSSSSSS".*, // 21
};

const SPLIT_2 = "PPPPPPHHHH".*;
const SPLIT_3 = "PPPPPPHHHH".*;
const SPLIT_4 = "HHHPPHHHHH".*;
const SPLIT_6 = "PPPPPHHHHH".*;
const SPLIT_7 = "PPPPPPHHHH".*;
const SPLIT_8 = "PPPPPPPPPP".*;
const SPLIT_9 = "PPPPPSPPSS".*;
const SPLIT_11 = "PPPPPPPPPP".*;

pub fn hardAction(player_value: i32, dealer_value: i32) u8 {
    if (player_value < 2 or player_value > 21 or dealer_value < 2 or dealer_value > 11) {
        return 'H';
    }
    return HARD_TABLE[@as(usize, @intCast(player_value - 2))][@as(usize, @intCast(dealer_value - 2))];
}

pub fn softAction(player_value: i32, dealer_value: i32) u8 {
    if (player_value < 13 or player_value > 21 or dealer_value < 2 or dealer_value > 11) {
        return 'H';
    }
    return SOFT_TABLE[@as(usize, @intCast(player_value - 13))][@as(usize, @intCast(dealer_value - 2))];
}

pub fn splitAction(split_value: i32, dealer_value: i32) u8 {
    if (dealer_value < 2 or dealer_value > 11) {
        return 'H';
    }
    const idx = @as(usize, @intCast(dealer_value - 2));
    return switch (split_value) {
        2 => SPLIT_2[idx],
        3 => SPLIT_3[idx],
        4 => SPLIT_4[idx],
        6 => SPLIT_6[idx],
        7 => SPLIT_7[idx],
        8 => SPLIT_8[idx],
        9 => SPLIT_9[idx],
        11 => SPLIT_11[idx],
        else => 'H',
    };
}
