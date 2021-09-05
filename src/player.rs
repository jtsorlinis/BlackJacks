use crate::card::Card;

static MAXSPLITS: i32 = 10;

pub struct Player {
    pub m_value: i32,
    pub m_earnings: f32,
    pub m_aces: i32,
    pub m_issoft: bool,
    pub m_splitcount: i32,
    pub m_isdone: bool,
    pub m_betmult: f32,
    pub m_hasnatural: bool,
    pub m_table: bool,
    pub m_initialbet: i32,
    pub m_originalbet: i32,
    pub m_hand: Vec<*mut Card>,
    pub m_playernum: String,
}

impl Player {
    pub fn new(playernum: &str, betsize: i32, splitcount: i32) -> Player {
        Player {
            m_value: 0,
            m_earnings: 0.0,
            m_aces: 0,
            m_issoft: false,
            m_splitcount: splitcount,
            m_isdone: false,
            m_betmult: 1.0,
            m_hasnatural: false,
            m_table: true,
            m_initialbet: betsize,
            m_originalbet: betsize,
            m_hand: Vec::with_capacity(5),
            m_playernum: playernum.to_owned(),
        }
    }

    pub fn double_bet(&mut self) {
        self.m_betmult = 2.0;
    }

    pub fn reset_hand(&mut self) {
        self.m_hand.clear();
        self.m_value = 0;
        self.m_aces = 0;
        self.m_issoft = false;
        self.m_splitcount = 0;
        self.m_isdone = false;
        self.m_betmult = 1.0;
        self.m_hasnatural = false;
        self.m_initialbet = self.m_originalbet
    }

    pub fn can_split(&self) -> i32 {
        unsafe {
            if self.m_hand.len() == 2
                && (*self.m_hand[0]).m_rank == (*self.m_hand[1]).m_rank
                && self.m_splitcount < MAXSPLITS
            {
                (*self.m_hand[0]).m_value
            } else {
                0
            }
        }
    }

    pub fn win(&mut self, mult: f32) -> f32 {
        let x = self.m_initialbet as f32 * self.m_betmult * mult;
        self.m_earnings += x;
        -x
    }

    pub fn lose(&mut self) -> f32 {
        let x = self.m_initialbet as f32 * self.m_betmult;
        self.m_earnings -= x;
        x
    }

    pub fn print(&self) -> String {
        unsafe {
            let mut output = "Player ".to_owned();
            output += &self.m_playernum;
            output += ": ";
            for &card in self.m_hand.iter() {
                output += (*card).print();
                output += " ";
            }
            for _ in self.m_hand.len()..5 {
                output += "  ";
            }
            output += "\tScore: ";
            output += &self.m_value.to_string();
            if self.m_value > 21 {
                output += " (Bust) ";
            } else {
                output += "        ";
            }
            output += "\tBet: ";
            output += &(self.m_initialbet as f32 * self.m_betmult).to_string();
            output
        }
    }

    pub fn evaluate(&mut self) {
        unsafe {
            self.m_aces = 0;
            self.m_value = 0;
            for &card in self.m_hand.iter() {
                self.m_value += (*card).m_value;
                if (*card).m_isace {
                    self.m_aces += 1;
                    self.m_issoft = true;
                }
            }

            while self.m_value > 21 && self.m_aces > 0 {
                self.m_value -= 10;
                self.m_aces -= 1;
            }

            if self.m_aces == 0 {
                self.m_issoft = false;
            }
        }
    }
}
