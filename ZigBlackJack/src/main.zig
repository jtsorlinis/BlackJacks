const std = @import("std");
const constants = @import("constants.zig");
const Table = @import("table.zig").Table;

pub fn main() !void {
    var gpa = std.heap.GeneralPurposeAllocator(.{}){};
    defer std.debug.assert(gpa.deinit() == .ok);
    const allocator = gpa.allocator();

    const args = try std.process.argsAlloc(allocator);
    defer std.process.argsFree(allocator, args);

    var rounds: i32 = constants.default_rounds;
    if (args.len == 2) {
        rounds = try std.fmt.parseInt(i32, args[1], 10);
    }

    var table = try Table.init(
        allocator,
        constants.default_num_players,
        constants.default_num_decks,
        constants.default_bet_size,
        constants.default_min_cards,
        constants.default_verbose,
    );
    defer table.deinit();

    table.card_pile.shuffle();

    const stderr = std.fs.File.stderr().deprecatedWriter();
    const stdout = std.fs.File.stdout().deprecatedWriter();
    const start_ns = std.time.nanoTimestamp();

    var x: i32 = 1;
    while (x <= rounds) : (x += 1) {
        if (!constants.default_verbose and rounds > 1000) {
            const every = @divTrunc(rounds, 100);
            if (@mod(x, every) == 0) {
                try stderr.print("\rProgress: {d}%", .{@divTrunc(x * 100, rounds)});
            }
        }

        try table.startRound();
        try table.checkEarnings();
    }

    table.clear();
    if (!constants.default_verbose and rounds > 1000) {
        try stderr.print("\r", .{});
    }

    const base_total = @as(f64, @floatFromInt(rounds * constants.default_bet_size));
    for (table.players.items, 0..) |player, idx| {
        const win_percentage = 50.0 + (player.earnings / base_total * 50.0);
        const earnings_i32 = @as(i32, @intFromFloat(player.earnings));
        try stdout.print(
            "Player {d} earnings: {d}\t\tWin Percentage: {d:.2}%\n",
            .{ idx + 1, earnings_i32, win_percentage },
        );
    }

    try stdout.print("Casino earnings: {d:.2}\n", .{table.casino_earnings});

    const elapsed_ns = std.time.nanoTimestamp() - start_ns;
    const elapsed_seconds = @as(f64, @floatFromInt(elapsed_ns)) / 1_000_000_000.0;
    try stdout.print("Played {d} rounds in {d:.3} seconds\n", .{ rounds, elapsed_seconds });
}
