const Card = @import("card.zig").Card;

pub const Deck = struct {
    cards: [52]Card,

    pub fn init() Deck {
        var cards: [52]Card = undefined;
        var idx: usize = 0;

        var suit: usize = 0;
        while (suit < 4) : (suit += 1) {
            var rank: u8 = 1;
            while (rank <= 13) : (rank += 1) {
                cards[idx] = Card.fromRank(rank);
                idx += 1;
            }
        }

        return .{ .cards = cards };
    }
};
