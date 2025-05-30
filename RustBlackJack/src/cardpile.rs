use crate::card::Card;
use crate::deck::Deck;
use std::time;

pub struct CardPile {
    #[allow(dead_code)]
    pub m_decks: i32,
    pub m_cards: Vec<*mut Card>,
    pub m_original_cards: Vec<*mut Card>,
    pub state: u64,
}

impl CardPile {
    pub fn new(decks: i32) -> CardPile {
        let mut c = Vec::<*mut Card>::with_capacity(decks as usize * 52);
        for _ in 0..decks {
            let mut temp = Deck::new();
            c.append(&mut temp.m_cards);
        }
        let mut cp = CardPile {
            m_decks: decks,
            m_original_cards: c.clone(),
            m_cards: c,
            state: time::SystemTime::now()
                .duration_since(time::SystemTime::UNIX_EPOCH)
                .unwrap()
                .as_secs(),
        };

        cp.refresh();

        cp
    }

    // From https://github.com/lemire/testingRNG
    fn wyrand(&mut self) -> u64 {
        self.state = self.state.wrapping_add(0xa0761d6478bd642f);
        let t: u128 = (self.state as u128).wrapping_mul((self.state ^ 0xe7037ed1a0b428db) as u128);
        (t.wrapping_shr(64) ^ t) as u64
    }

    // use nearly divisionless technique found here https://github.com/lemire/FastShuffleExperiments
    fn rand_range(&mut self, s: u64) -> u64 {
        let mut x = self.wyrand();
        let mut m = x as u128 * s as u128;
        let mut l = m as u64;
        if l < s {
            let thresh = s.wrapping_neg() % s;
            while l < thresh {
                x = self.wyrand();
                m = x as u128 * s as u128;
                l = m as u64;
            }
        }
        (m >> 64) as u64
    }

    // pub fn print(&self) -> String {
    //     self.m_cards[0].print();
    //     let mut output = String::default();
    //     for card in self.m_cards.iter() {
    //         output += card.print();
    //     }
    //     return output;
    // }

    pub fn shuffle(&mut self) {
        for i in (1..self.m_cards.len()).rev() {
            let j = self.rand_range((i + 1) as u64) as usize;
            self.m_cards.swap(i, j);
        }
    }

    pub fn refresh(&mut self) {
        self.m_cards = self.m_original_cards.clone();
    }
}
