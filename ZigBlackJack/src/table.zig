const std = @import("std");
const constants = @import("constants.zig");
const Player = @import("player.zig").Player;
const Dealer = @import("dealer.zig").Dealer;
const CardPile = @import("cardpile.zig").CardPile;
const strategies = @import("strategies.zig");

pub const Table = struct {
    allocator: std.mem.Allocator,
    verbose: bool,
    bet_size: i32,
    players: std.ArrayList(Player),
    num_decks: i32,
    card_pile: CardPile,
    min_cards: usize,
    dealer: Dealer,
    current_player: usize,
    casino_earnings: f64,
    running_count: i32,
    true_count: i32,

    pub fn init(
        allocator: std.mem.Allocator,
        num_players: i32,
        num_decks: i32,
        bet_size: i32,
        min_cards: usize,
        verbose: bool,
    ) !Table {
        const player_capacity = @as(usize, @intCast(num_players * 3));
        var players = try std.ArrayList(Player).initCapacity(allocator, player_capacity);

        var i: i32 = 0;
        while (i < num_players) : (i += 1) {
            try players.append(allocator, Player.init(bet_size, 0));
        }

        return .{
            .allocator = allocator,
            .verbose = verbose,
            .bet_size = bet_size,
            .players = players,
            .num_decks = num_decks,
            .card_pile = try CardPile.init(allocator, @as(usize, @intCast(num_decks))),
            .min_cards = min_cards,
            .dealer = Dealer.init(),
            .current_player = 0,
            .casino_earnings = 0,
            .running_count = 0,
            .true_count = 0,
        };
    }

    pub fn deinit(self: *Table) void {
        self.players.deinit(self.allocator);
        self.card_pile.deinit();
    }

    pub fn startRound(self: *Table) !void {
        self.updateCount();

        if (self.verbose) {
            const stderr = std.fs.File.stderr().deprecatedWriter();
            try stderr.print("{d} cards left\n", .{self.card_pile.cards.items.len});
            try stderr.print("Running count is: {d}\tTrue count is: {d}\n", .{ self.running_count, self.true_count });
        }

        try self.getNewCards();
        self.preDeal();
        self.dealRound();
        self.dealDealer(false);
        self.dealRound();
        self.dealDealer(true);
        self.evaluateAll();
        self.current_player = 0;

        if (self.checkDealerNatural()) {
            try self.finishRound();
        } else {
            self.checkPlayerNatural();
            try self.autoPlay();
        }
    }

    pub fn checkEarnings(self: *const Table) !void {
        var check: f64 = 0;
        for (self.players.items) |player| {
            check += player.earnings;
        }

        if (@abs(check + self.casino_earnings) > 0.000001) {
            return error.EarningsMismatch;
        }
    }

    pub fn clear(self: *Table) void {
        var idx = @as(isize, @intCast(self.players.items.len)) - 1;
        while (idx >= 0) : (idx -= 1) {
            const i = @as(usize, @intCast(idx));
            if (self.players.items[i].split_count > 0) {
                self.players.items[i - 1].earnings += self.players.items[i].earnings;
                _ = self.players.orderedRemove(i);
            } else {
                self.players.items[i].resetHand();
            }
        }

        self.dealer.resetHand();
        self.current_player = 0;
    }

    fn dealRound(self: *Table) void {
        const count = self.players.items.len;
        var i: usize = 0;
        while (i < count) : (i += 1) {
            self.deal();
            self.current_player += 1;
        }
        self.current_player = 0;
    }

    fn evaluateAll(self: *Table) void {
        for (self.players.items) |*player| {
            _ = player.evaluate();
        }
    }

    fn deal(self: *Table) void {
        const card = self.card_pile.pop();
        self.running_count += card.count;
        self.players.items[self.current_player].pushCard(card);
    }

    fn preDeal(self: *Table) void {
        for (self.players.items) |*player| {
            self.selectBet(player);
        }
    }

    fn selectBet(self: *Table, player: *Player) void {
        if (self.true_count >= 2) {
            player.initial_bet = self.bet_size * (self.true_count - 1);
        }
    }

    fn dealDealer(self: *Table, face_down: bool) void {
        const card = self.card_pile.pop();
        if (!face_down) {
            self.running_count += card.count;
        }
        self.dealer.pushCard(card);
    }

    fn getNewCards(self: *Table) !void {
        if (self.card_pile.cards.items.len < self.min_cards) {
            try self.card_pile.refresh();
            self.card_pile.shuffle();
            self.true_count = 0;
            self.running_count = 0;
        }
    }

    fn updateCount(self: *Table) void {
        if (self.card_pile.cards.items.len > 51) {
            const decks_left = @as(i32, @intCast(self.card_pile.cards.items.len / 52));
            self.true_count = @divTrunc(self.running_count, decks_left);
        }
    }

    fn hit(self: *Table) void {
        self.deal();
        _ = self.players.items[self.current_player].evaluate();
    }

    fn stand(self: *Table) void {
        self.players.items[self.current_player].is_done = true;
    }

    fn split(self: *Table) !void {
        const current_index = self.current_player;
        const current = &self.players.items[current_index];
        const moved_card = current.popCard();

        var split_player = Player.init(current.initial_bet, current.split_count + 1);
        split_player.pushCard(moved_card);
        try self.players.insert(self.allocator, current_index + 1, split_player);

        _ = self.players.items[current_index].evaluate();
        _ = self.players.items[current_index + 1].evaluate();
    }

    fn splitAces(self: *Table) !void {
        const current_index = self.current_player;
        const current = &self.players.items[current_index];
        const moved_card = current.popCard();

        var split_player = Player.init(current.initial_bet, current.split_count + 1);
        split_player.pushCard(moved_card);
        try self.players.insert(self.allocator, current_index + 1, split_player);

        self.deal();
        _ = self.players.items[self.current_player].evaluate();
        self.stand();

        self.current_player += 1;
        self.deal();
        _ = self.players.items[self.current_player].evaluate();
        self.stand();
    }

    fn doubleBet(self: *Table) !void {
        const player = &self.players.items[self.current_player];
        if (player.bet_mult < 1.1 and player.hand_len == 2) {
            player.doubleBet();
            self.hit();
            self.stand();
        } else {
            self.hit();
        }
    }

    fn autoPlay(self: *Table) !void {
        while (self.current_player < self.players.items.len) {
            while (!self.players.items[self.current_player].is_done) {
                if (self.players.items[self.current_player].hand_len == 1) {
                    self.deal();
                    _ = self.players.items[self.current_player].evaluate();
                }

                if (self.players.items[self.current_player].hand_len < constants.max_hand_cards and
                    self.players.items[self.current_player].value < 21)
                {
                    const split_value = self.players.items[self.current_player].canSplit();
                    const dealer_up = self.dealer.upCard();

                    if (split_value == 11) {
                        try self.splitAces();
                    } else if (split_value != 0 and split_value != 5 and split_value != 10) {
                        try self.action(strategies.splitAction(split_value, dealer_up));
                    } else if (self.players.items[self.current_player].is_soft) {
                        try self.action(strategies.softAction(self.players.items[self.current_player].value, dealer_up));
                    } else {
                        try self.action(strategies.hardAction(self.players.items[self.current_player].value, dealer_up));
                    }
                } else {
                    self.stand();
                }
            }
            self.current_player += 1;
        }

        self.current_player = 0;
        try self.dealerPlay();
    }

    fn action(self: *Table, action_code: u8) !void {
        switch (action_code) {
            'H' => self.hit(),
            'S' => self.stand(),
            'D' => try self.doubleBet(),
            'P' => try self.split(),
            else => return error.InvalidStrategyAction,
        }
    }

    fn dealerPlay(self: *Table) !void {
        var all_busted = true;
        for (self.players.items) |player| {
            if (player.value < 22) {
                all_busted = false;
                break;
            }
        }

        self.dealer.hide_second = false;
        self.running_count += self.dealer.hand[1].count;
        _ = self.dealer.evaluate();

        if (!all_busted) {
            while (self.dealer.value < 17 and self.dealer.hand_len < constants.max_hand_cards) {
                self.dealDealer(false);
                _ = self.dealer.evaluate();
            }
        }

        try self.finishRound();
    }

    fn checkPlayerNatural(self: *Table) void {
        for (self.players.items) |*player| {
            if (player.value == 21 and player.hand_len == 2 and player.split_count == 0) {
                player.has_natural = true;
            }
        }
    }

    fn checkDealerNatural(self: *Table) bool {
        if (self.dealer.evaluate() == 21) {
            self.dealer.hide_second = false;
            self.running_count += self.dealer.hand[1].count;
            return true;
        }
        return false;
    }

    fn finishRound(self: *Table) !void {
        for (self.players.items) |*player| {
            if (player.has_natural) {
                self.casino_earnings += player.win(1.5);
            } else if (player.value > 21) {
                self.casino_earnings += player.lose();
            } else if (self.dealer.value > 21 or player.value > self.dealer.value) {
                self.casino_earnings += player.win(1);
            } else if (player.value == self.dealer.value) {
                // Push
            } else {
                self.casino_earnings += player.lose();
            }
        }

        self.clear();
    }
};
