use crate::card::Card;
use std::cell::Cell;
use std::rc::Rc;

static MAXHANDS: i32 = 4;

pub struct Player {
    pub m_value: i32,
    pub m_earnings: f64,
    pub m_aces: i32,
    pub m_issoft: bool,
    pub m_splitcount: Rc<Cell<i32>>,
    pub m_issplithand: bool,
    pub m_isdone: bool,
    pub m_betmult: f64,
    pub m_hasnatural: bool,
    #[allow(dead_code)]
    pub m_table: bool,
    pub m_initialbet: i32,
    pub m_originalbet: i32,
    pub m_hand: Vec<*mut Card>,
    pub m_playernum: String,
}

impl Player {
    pub fn new(playernum: &str, betsize: i32) -> Player {
        Player {
            m_value: 0,
            m_earnings: 0.0,
            m_aces: 0,
            m_issoft: false,
            m_splitcount: Rc::new(Cell::new(0)),
            m_issplithand: false,
            m_isdone: false,
            m_betmult: 1.0,
            m_hasnatural: false,
            m_table: true,
            m_initialbet: betsize,
            m_originalbet: betsize,
            m_hand: Vec::<*mut Card>::with_capacity(5),
            m_playernum: playernum.to_owned(),
        }
    }

    pub fn new_split(
        playernum: String,
        initialbet: i32,
        originalbet: i32,
        splitcount: Rc<Cell<i32>>,
    ) -> Player {
        Player {
            m_value: 0,
            m_earnings: 0.0,
            m_aces: 0,
            m_issoft: false,
            m_splitcount: splitcount,
            m_issplithand: true,
            m_isdone: false,
            m_betmult: 1.0,
            m_hasnatural: false,
            m_table: true,
            m_initialbet: initialbet,
            m_originalbet: originalbet,
            m_hand: Vec::<*mut Card>::with_capacity(5),
            m_playernum: playernum,
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
        self.m_splitcount.set(0);
        self.m_isdone = false;
        self.m_betmult = 1.0;
        self.m_hasnatural = false;
        self.m_initialbet = self.m_originalbet
    }

    pub fn record_split(&self) {
        self.m_splitcount.set(self.m_splitcount.get() + 1);
    }

    pub fn has_split(&self) -> bool {
        self.m_splitcount.get() > 0
    }

    pub fn can_split(&self) -> i32 {
        unsafe {
            if self.m_hand.len() == 2
                && (*self.m_hand[0]).m_rankid == (*self.m_hand[1]).m_rankid
                && self.m_splitcount.get() < MAXHANDS - 1
            {
                (*self.m_hand[0]).m_value
            } else {
                0
            }
        }
    }

    pub fn win(&mut self, mult: f64) -> f64 {
        let x = self.m_initialbet as f64 * self.m_betmult * mult;
        self.m_earnings += x;
        -x
    }

    pub fn lose(&mut self) -> f64 {
        let x = self.m_initialbet as f64 * self.m_betmult;
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
            output += &(self.m_initialbet as f64 * self.m_betmult).to_string();
            output
        }
    }

    pub fn evaluate(&mut self) {
        unsafe {
            let mut value = 0;
            let mut aces = 0;
            for &card in self.m_hand.iter() {
                value += (*card).m_value;
                aces += (*card).m_isace as i32;
            }

            while value > 21 && aces > 0 {
                value -= 10;
                aces -= 1;
            }

            self.m_value = value;
            self.m_aces = aces;
            self.m_issoft = aces > 0;
        }
    }
}
