pub const Card = struct {
    rank: u8,
    value: i32,
    count: i32,
    is_ace: bool,

    pub fn fromRank(rank: u8) Card {
        const value: i32 = if (rank == 1) 11 else if (rank >= 10) 10 else @as(i32, rank);
        const count: i32 = if (rank == 1 or rank >= 10)
            -1
        else if (rank >= 7 and rank <= 9)
            0
        else
            1;

        return .{
            .rank = rank,
            .value = value,
            .count = count,
            .is_ace = rank == 1,
        };
    }
};
