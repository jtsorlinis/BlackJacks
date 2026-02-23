const std = @import("std");
const Card = @import("card.zig").Card;
const Deck = @import("deck.zig").Deck;

const WyRand = struct {
    state: u64,

    fn init() WyRand {
        const now = std.time.nanoTimestamp();
        const seed = @as(u64, @truncate(@as(u128, @bitCast(now))));
        return .{ .state = seed ^ 0xa0761d6478bd642f };
    }

    fn next(self: *WyRand) u64 {
        self.state +%= 0xa0761d6478bd642f;
        const t: u128 = @as(u128, self.state) * @as(u128, self.state ^ 0xe7037ed1a0b428db);
        return @as(u64, @truncate((t >> 64) ^ t));
    }

    fn randRange(self: *WyRand, s: u64) u64 {
        var x = self.next();
        var m: u128 = @as(u128, x) * @as(u128, s);
        var l = @as(u64, @truncate(m));

        if (l < s) {
            const threshold = (0 -% s) % s;
            while (l < threshold) {
                x = self.next();
                m = @as(u128, x) * @as(u128, s);
                l = @as(u64, @truncate(m));
            }
        }

        return @as(u64, @truncate(m >> 64));
    }
};

pub const CardPile = struct {
    allocator: std.mem.Allocator,
    cards: std.ArrayList(Card),
    original_cards: std.ArrayList(Card),
    rng: WyRand,

    pub fn init(allocator: std.mem.Allocator, num_decks: usize) !CardPile {
        var cards = try std.ArrayList(Card).initCapacity(allocator, num_decks * 52);

        var deck: usize = 0;
        while (deck < num_decks) : (deck += 1) {
            const temp_deck = Deck.init();
            try cards.appendSlice(allocator, temp_deck.cards[0..]);
        }

        var original_cards = try std.ArrayList(Card).initCapacity(allocator, cards.items.len);
        try original_cards.appendSlice(allocator, cards.items);

        return .{
            .allocator = allocator,
            .cards = cards,
            .original_cards = original_cards,
            .rng = WyRand.init(),
        };
    }

    pub fn deinit(self: *CardPile) void {
        self.cards.deinit(self.allocator);
        self.original_cards.deinit(self.allocator);
    }

    pub fn refresh(self: *CardPile) !void {
        self.cards.clearRetainingCapacity();
        try self.cards.appendSlice(self.allocator, self.original_cards.items);
    }

    pub fn pop(self: *CardPile) Card {
        const len = self.cards.items.len;
        std.debug.assert(len > 0);
        const card = self.cards.items[len - 1];
        self.cards.items.len = len - 1;
        return card;
    }

    pub fn shuffle(self: *CardPile) void {
        if (self.cards.items.len <= 1) {
            return;
        }

        var i = self.cards.items.len - 1;
        while (i > 0) : (i -= 1) {
            const j = @as(usize, @intCast(self.rng.randRange(@as(u64, @intCast(i + 1)))));
            const tmp = self.cards.items[i];
            self.cards.items[i] = self.cards.items[j];
            self.cards.items[j] = tmp;
        }
    }
};
