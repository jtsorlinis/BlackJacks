const std = @import("std");
const constants = @import("constants.zig");
const Card = @import("card.zig").Card;

pub const Dealer = struct {
    value: i32,
    aces: i32,
    is_soft: bool,
    hand: [constants.max_hand_cards]Card,
    hand_len: usize,
    hide_second: bool,

    pub fn init() Dealer {
        return .{
            .value = 0,
            .aces = 0,
            .is_soft = false,
            .hand = undefined,
            .hand_len = 0,
            .hide_second = true,
        };
    }

    pub fn pushCard(self: *Dealer, card: Card) void {
        std.debug.assert(self.hand_len < constants.max_hand_cards);
        self.hand[self.hand_len] = card;
        self.hand_len += 1;
    }

    pub fn upCard(self: *const Dealer) i32 {
        std.debug.assert(self.hand_len > 0);
        return self.hand[0].value;
    }

    pub fn evaluate(self: *Dealer) i32 {
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

    pub fn resetHand(self: *Dealer) void {
        self.value = 0;
        self.aces = 0;
        self.is_soft = false;
        self.hand_len = 0;
        self.hide_second = true;
    }
};
