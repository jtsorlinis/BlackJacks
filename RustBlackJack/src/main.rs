mod card;
mod cardpile;
mod dealer;
mod deck;
mod player;
mod strategies;
mod table;

use mimalloc::MiMalloc;
use std::env;
use std::time::Instant;
use table::Table;

#[global_allocator]
static GLOBAL: MiMalloc = MiMalloc;

fn main() {
    let args: Vec<String> = env::args().collect();

    const NUM_PLAYERS: i32 = 5;
    const NUM_DECKS: i32 = 8;
    const BET_SIZE: i32 = 10;
    const MIN_CARDS: usize = 52;

    let mut rounds: i32 = 1000000;
    const VERBOSE: bool = false;

    if args.len() == 2 {
        rounds = args[1].parse::<i32>().unwrap();
    }

    let mut table1 = Table::new(NUM_PLAYERS, NUM_DECKS, BET_SIZE, MIN_CARDS, VERBOSE);
    table1.m_cardpile.shuffle();

    let start = Instant::now();

    for x in 1..rounds + 1 {
        if VERBOSE {
            println!("Round {}", x);
        }
        if !VERBOSE && rounds > 1000 && x % (rounds / 100) == 0 {
            eprint!("\tProgress: {}%\r", x * 100 / rounds);
        }

        table1.start_round();
        table1.check_earnings();
    }

    table1.clear();
    for player in table1.m_players.iter() {
        println!(
            "Player {} earnings: {}\t\tWin Percentage: {:.2}%",
            player.m_playernum,
            player.m_earnings as i32,
            50.0 + f64::from(player.m_earnings) / (f64::from(rounds) * f64::from(BET_SIZE)) * 50.0
        );
    }
    println!("Casino earnings: {}", table1.m_casinoearnings as i32);
    println!(
        "Played {} rounds in {:.3} seconds",
        rounds,
        start.elapsed().as_secs_f64()
    );
}
