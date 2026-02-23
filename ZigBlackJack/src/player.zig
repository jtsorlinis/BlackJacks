const std = @import("std");
const constants = @import("constants.zig");
const Card = @import("card.zig").Card;

pub const Player = struct {
    value: i32,
    earnings: f64,
    aces: i32,
    is_soft: bool,
    split_count: i32,
    is_done: bool,
    bet_mult: f64,
    has_natural: bool,
    initial_bet: i32,
    original_bet: i32,
    hand: [constants.max_hand_cards]Card,
    hand_len: usize,

    pub fn init(base_bet: i32, split_count: i32) Player {
        return .{
            .value = 0,
            .earnings = 0,
            .aces = 0,
            .is_soft = false,
            .split_count = split_count,
            .is_done = false,
            .bet_mult = 1,
            .has_natural = false,
            .initial_bet = base_bet,
            .original_bet = base_bet,
            .hand = undefined,
            .hand_len = 0,
        };
    }

    pub fn pushCard(self: *Player, card: Card) void {
        std.debug.assert(self.hand_len < constants.max_hand_cards);
        self.hand[self.hand_len] = card;
        self.hand_len += 1;
    }

    pub fn popCard(self: *Player) Card {
        std.debug.assert(self.hand_len > 0);
        self.hand_len -= 1;
        return self.hand[self.hand_len];
    }

    pub fn evaluate(self: *Player) i32 {
        self.value = 0;
        self.aces = 0;
        self.is_soft = false;

        var i: usize = 0;
        while (i < self.hand_len) : (i += 1) {
            const card = self.hand[i];
            self.value += card.value;
            if (card.is_ace) {
                self.aces += 1;
                self.is_soft = true;
            }
        }

        while (self.value > 21 and self.aces > 0) {
            self.value -= 10;
            self.aces -= 1;
        }

        if (self.aces == 0) {
            self.is_soft = false;
        }

        return self.value;
    }

    pub fn resetHand(self: *Player) void {
        self.value = 0;
        self.aces = 0;
        self.is_soft = false;
        self.split_count = 0;
        self.is_done = false;
        self.bet_mult = 1;
        self.has_natural = false;
        self.initial_bet = self.original_bet;
        self.hand_len = 0;
    }

    pub fn canSplit(self: *const Player) i32 {
        if (self.hand_len == 2 and self.hand[0].rank == self.hand[1].rank and self.split_count < constants.max_splits) {
            return self.hand[0].value;
        }
        return 0;
    }

    pub fn doubleBet(self: *Player) void {
        self.bet_mult = 2;
    }

    pub fn win(self: *Player, mult: f64) f64 {
        const amount = @as(f64, @floatFromInt(self.initial_bet)) * self.bet_mult * mult;
        self.earnings += amount;
        return -amount;
    }

    pub fn lose(self: *Player) f64 {
        const amount = @as(f64, @floatFromInt(self.initial_bet)) * self.bet_mult;
        self.earnings -= amount;
        return amount;
    }
};
