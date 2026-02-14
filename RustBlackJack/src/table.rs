use crate::cardpile::CardPile;
use crate::dealer::Dealer;
use crate::player::Player;
use crate::strategies;

pub struct Table {
    pub m_verbose: bool,
    pub m_betsize: i32,
    pub m_players: Vec<Player>,
    pub m_numofdecks: i32,
    pub m_cardpile: CardPile,
    pub m_mincards: usize,
    pub m_dealer: Dealer,
    pub m_currentplayer: usize,
    pub m_casinoearnings: f32,
    pub m_runningcount: i32,
    pub m_truecount: i32,
}

impl Table {
    pub fn new(
        numplayers: i32,
        numdecks: i32,
        betsize: i32,
        mincards: usize,
        verbose: bool,
    ) -> Table {
        Table {
            m_verbose: verbose,
            m_betsize: betsize,
            m_players: Table::fill(numplayers, betsize),
            m_numofdecks: numdecks,
            m_cardpile: CardPile::new(numdecks),
            m_mincards: mincards,
            m_dealer: Dealer::new(),
            m_currentplayer: 0,
            m_casinoearnings: 0.0,
            m_runningcount: 0,
            m_truecount: 0,
        }
    }

    fn fill(numplayers: i32, betsize: i32) -> Vec<Player> {
        let mut temp: Vec<Player> = Vec::with_capacity((numplayers * 3) as usize);
        for i in 0..numplayers {
            temp.push(Player::new(&(i + 1).to_string(), betsize, 0));
        }
        temp
    }

    fn deal_round(&mut self) {
        for _ in 0..self.m_players.len() {
            self.deal();
            self.m_currentplayer += 1;
        }
        self.m_currentplayer = 0;
    }

    fn evaluate_all(&mut self) {
        for i in 0..self.m_players.len() {
            self.m_players[i].evaluate();
        }
    }

    pub fn start_round(&mut self) {
        self.update_count();
        if self.m_verbose {
            println!("{} cards left", self.m_cardpile.m_cards.len());
            println!(
                "Running count is: {}\tTrue count is: {}",
                self.m_runningcount, self.m_truecount
            );
        }
        self.get_new_cards();
        self.predeal();
        self.deal_round();
        self.deal_dealer(false);
        self.deal_round();
        self.deal_dealer(true);
        self.evaluate_all();
        self.m_currentplayer = 0;
        if self.check_dealer_natural() {
            self.finish_round();
        } else {
            self.check_player_natural();
            if self.m_verbose {
                self.print();
            }
            self.autoplay();
        }
    }

    fn deal(&mut self) {
        unsafe {
            let tempcard = self.m_cardpile.m_cards.pop().unwrap();
            self.m_runningcount += (*tempcard).m_count;
            self.m_players[self.m_currentplayer].m_hand.push(tempcard);
        }
    }

    fn predeal(&mut self) {
        self.m_currentplayer = 0;
        while self.m_currentplayer < self.m_players.len() {
            self.select_bet();
            self.m_currentplayer += 1;
        }
        self.m_currentplayer = 0;
    }

    fn select_bet(&mut self) {
        if self.m_truecount >= 2 {
            self.m_players[self.m_currentplayer].m_initialbet =
                self.m_betsize * (self.m_truecount - 1);
        }
    }

    fn deal_dealer(&mut self, facedown: bool) {
        unsafe {
            let tempcard = self.m_cardpile.m_cards.pop().unwrap();
            if !facedown {
                self.m_runningcount += (*tempcard).m_count;
            }
            self.m_dealer.m_hand.push(tempcard);
        }
    }

    fn get_new_cards(&mut self) {
        if self.m_cardpile.m_cards.len() < self.m_mincards {
            self.m_cardpile.refresh();
            self.m_cardpile.shuffle();
            self.m_truecount = 0;
            self.m_runningcount = 0;
            if self.m_verbose {
                println!(
                    "Got {} new decks as number of cards left is below {}",
                    self.m_numofdecks, self.m_mincards
                )
            }
        }
    }

    pub fn clear(&mut self) {
        for player in (0..self.m_players.len()).rev() {
            if self.m_players[player].m_splitcount > 0 {
                self.m_players[player - 1].m_earnings += self.m_players[player].m_earnings;
                self.m_players.remove(player);
            } else {
                self.m_players[player].reset_hand();
            }
        }
        self.m_dealer.reset_hand();
        self.m_currentplayer = 0;
    }

    fn update_count(&mut self) {
        if self.m_cardpile.m_cards.len() > 51 {
            self.m_truecount = self.m_runningcount / (self.m_cardpile.m_cards.len() as i32 / 52);
        }
    }

    fn hit(&mut self) {
        self.deal();
        self.m_players[self.m_currentplayer].evaluate();
        if self.m_verbose {
            println!(
                "Player {} hits",
                self.m_players[self.m_currentplayer].m_playernum
            );
        }
    }

    fn stand(&mut self) {
        if self.m_verbose && self.m_players[self.m_currentplayer].m_value <= 21 {
            println!(
                "Player {} stands",
                self.m_players[self.m_currentplayer].m_playernum
            );
            self.print();
        }
        self.m_players[self.m_currentplayer].m_isdone = true;
    }

    fn split(&mut self) {
        let splitplayernum = (self.m_players[self.m_currentplayer].m_playernum).to_string() + "S";
        let mut splitplayer = Player::new(
            &splitplayernum,
            self.m_players[self.m_currentplayer].m_initialbet,
            self.m_players[self.m_currentplayer].m_splitcount + 1,
        );
        splitplayer
            .m_hand
            .push(self.m_players[self.m_currentplayer].m_hand.pop().unwrap());
        self.m_players.insert(self.m_currentplayer + 1, splitplayer);
        self.m_players[self.m_currentplayer].evaluate();
        self.m_players[self.m_currentplayer + 1].evaluate();
        if self.m_verbose {
            println!(
                "Player {} splits",
                self.m_players[self.m_currentplayer].m_playernum
            );
        }
    }

    fn split_aces(&mut self) {
        if self.m_verbose {
            println!(
                "Player {} splits aces",
                self.m_players[self.m_currentplayer].m_playernum
            );
        }
        let splitplayernum = (self.m_players[self.m_currentplayer].m_playernum).to_string() + "S";
        let mut splitplayer = Player::new(
            &splitplayernum,
            self.m_players[self.m_currentplayer].m_initialbet,
            self.m_players[self.m_currentplayer].m_splitcount + 1,
        );
        splitplayer
            .m_hand
            .push(self.m_players[self.m_currentplayer].m_hand.pop().unwrap());
        self.m_players.insert(self.m_currentplayer + 1, splitplayer);
        self.deal();
        self.m_players[self.m_currentplayer].evaluate();
        self.stand();
        self.m_currentplayer += 1;
        self.deal();
        self.m_players[self.m_currentplayer].evaluate();
        self.stand();
        if self.m_verbose {
            self.print();
        }
    }

    fn double_bet(&mut self) {
        if self.m_players[self.m_currentplayer].m_betmult < 1.1
            && self.m_players[self.m_currentplayer].m_hand.len() == 2
        {
            self.m_players[self.m_currentplayer].double_bet();
            if self.m_verbose {
                println!(
                    "Player {} doubles",
                    self.m_players[self.m_currentplayer].m_playernum
                );
            }
            self.hit();
            self.stand();
        } else {
            self.hit();
        }
    }

    fn autoplay(&mut self) {
        while self.m_currentplayer < self.m_players.len() {
            while !self.m_players[self.m_currentplayer].m_isdone {
                // check if player just split
                if self.m_players[self.m_currentplayer].m_hand.len() == 1 {
                    if self.m_verbose {
                        println!(
                            "Player {} gets 2nd card after splitting",
                            self.m_players[self.m_currentplayer].m_playernum
                        );
                    }
                    self.deal();
                    self.m_players[self.m_currentplayer].evaluate();
                }
                if self.m_players[self.m_currentplayer].m_hand.len() < 5
                    && self.m_players[self.m_currentplayer].m_value < 21
                {
                    let split_player_val = self.m_players[self.m_currentplayer].can_split();
                    let dealer_up = self.m_dealer.up_card();
                    if split_player_val == 11 {
                        self.split_aces();
                    } else if split_player_val != 0
                        && (split_player_val != 5 && split_player_val != 10)
                    {
                        self.action(strategies::get_action(
                            split_player_val,
                            dealer_up,
                            &strategies::MAP_SPLIT,
                        ));
                    } else if self.m_players[self.m_currentplayer].m_issoft {
                        self.action(strategies::get_action(
                            self.m_players[self.m_currentplayer].m_value,
                            dealer_up,
                            &strategies::MAP_SOFT,
                        ));
                    } else {
                        self.action(strategies::get_action(
                            self.m_players[self.m_currentplayer].m_value,
                            dealer_up,
                            &strategies::MAP_HARD,
                        ));
                    }
                } else {
                    self.stand();
                }
            }
            self.m_currentplayer += 1;
        }
        self.m_currentplayer = 0;
        self.dealer_play();
    }

    fn action(&mut self, action: char) {
        match action {
            'H' => self.hit(),
            'S' => self.stand(),
            'D' => self.double_bet(),
            'P' => self.split(),
            _ => {
                println!("No Action found");
                std::process::exit(1);
            }
        }
    }

    fn dealer_play(&mut self) {
        let mut allbusted = true;
        for player in self.m_players.iter() {
            if player.m_value < 22 {
                allbusted = false;
                break;
            }
        }
        self.m_dealer.m_hide = false;
        unsafe {
            self.m_runningcount += (*self.m_dealer.m_hand[1]).m_count;
        }
        self.m_dealer.evaluate();
        if self.m_verbose {
            println!("Dealer's turn");
            self.print();
        }
        if allbusted {
            if self.m_verbose {
                println!("Dealer automatically wins cause all players busted");
            }
        } else {
            while self.m_dealer.m_value < 17 && self.m_dealer.m_hand.len() < 5 {
                self.deal_dealer(false);
                self.m_dealer.evaluate();
                if self.m_verbose {
                    println!("Dealer hits");
                    self.print();
                }
            }
        }
        self.finish_round();
    }

    fn check_player_natural(&mut self) {
        for player in self.m_players.iter_mut() {
            if player.m_value == 21 && player.m_hand.len() == 2 && player.m_splitcount == 0 {
                player.m_hasnatural = true;
            }
        }
    }

    fn check_dealer_natural(&mut self) -> bool {
        self.m_dealer.evaluate();
        if self.m_dealer.m_value == 21 {
            self.m_dealer.m_hide = false;
            unsafe {
                self.m_runningcount += (*self.m_dealer.m_hand[1]).m_count;
            }
            if self.m_verbose {
                self.print();
                println!("Dealer has a natural 21");
            }
            return true;
        }
        false
    }

    pub fn check_earnings(&self) {
        let mut check = 0.0;
        for player in self.m_players.iter() {
            check += player.m_earnings;
        }
        if check + self.m_casinoearnings != 0.0 {
            println!(
                "Earnings dont match! Player total: {}, Casino total: {}",
                check, self.m_casinoearnings
            );
            std::process::exit(1);
        }
    }

    fn finish_round(&mut self) {
        if self.m_verbose {
            println!("Scoring round");
        }
        for player in self.m_players.iter_mut() {
            if player.m_hasnatural {
                self.m_casinoearnings += player.win(1.5);
                if self.m_verbose {
                    println!(
                        "Player {} Wins {} with a natural 21",
                        player.m_playernum,
                        1.5 * player.m_betmult * player.m_initialbet as f32
                    );
                }
            } else if player.m_value > 21 {
                self.m_casinoearnings += player.lose();
                if self.m_verbose {
                    println!(
                        "Player {} Busts and Loses {}",
                        player.m_playernum,
                        player.m_betmult * player.m_initialbet as f32
                    );
                }
            } else if self.m_dealer.m_value > 21 || player.m_value > self.m_dealer.m_value {
                self.m_casinoearnings += player.win(1.0);
                if self.m_verbose {
                    println!(
                        "Player {} Wins {}",
                        player.m_playernum,
                        player.m_betmult * player.m_initialbet as f32
                    )
                }
            } else if player.m_value == self.m_dealer.m_value {
                if self.m_verbose {
                    println!("Player {} Draws", player.m_playernum)
                }
            } else {
                self.m_casinoearnings += player.lose();
                if self.m_verbose {
                    println!(
                        "Player {} Loses {}",
                        player.m_playernum,
                        player.m_betmult * player.m_initialbet as f32
                    )
                }
            }
        }
        if self.m_verbose {
            for player in self.m_players.iter() {
                if player.m_splitcount == 0 {
                    println!(
                        "Player {} Earnings: {}",
                        player.m_playernum, player.m_earnings
                    );
                }
            }
            println!();
        }
        self.clear();
    }

    fn print(&self) {
        for player in self.m_players.iter() {
            println!("{}", player.print());
        }
        println!("{}", self.m_dealer.print());
        println!();
    }
}
