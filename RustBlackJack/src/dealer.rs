use crate::card::Card;

pub struct Dealer {
    pub m_value: i32,
    pub m_aces: i32,
    pub m_issoft: bool,
    pub m_hand: Vec<*mut Card>,
    pub m_playernum: String,
    pub m_hide: bool,
}

impl Dealer {
    pub fn new() -> Dealer {
        Dealer {
            m_value: 0,
            m_aces: 0,
            m_issoft: false,
            m_hand: Vec::<*mut Card>::with_capacity(5),
            m_playernum: "D".to_owned(),
            m_hide: true,
        }
    }

    pub fn up_card(&self) -> i32 {
        unsafe { (*self.m_hand[0]).m_value }
    }

    pub fn reset_hand(&mut self) {
        self.m_hand.clear();
        self.m_value = 0;
        self.m_aces = 0;
        self.m_issoft = false;
        self.m_hide = true;
    }

    pub fn print(&self) -> String {
        unsafe {
            let mut output = "Player ".to_owned();
            output += &self.m_playernum;
            output += ": ";
            for card in 0..self.m_hand.len() {
                if card == 1 && self.m_hide {
                    output += "X";
                } else {
                    output += (*self.m_hand[card]).print();
                }

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
