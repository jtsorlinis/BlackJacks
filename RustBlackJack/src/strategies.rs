lazy_static! {
    pub static ref STRAT_HARD: Vec<Vec<&'static str>> = vec![
        vec!["0", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11"],
        vec!["2", "H", "H", "H", "H", "H", "H", "H", "H", "H", "H"],
        vec!["3", "H", "H", "H", "H", "H", "H", "H", "H", "H", "H"],
        vec!["4", "H", "H", "H", "H", "H", "H", "H", "H", "H", "H"],
        vec!["5", "H", "H", "H", "H", "H", "H", "H", "H", "H", "H"],
        vec!["6", "H", "H", "H", "H", "H", "H", "H", "H", "H", "H"],
        vec!["7", "H", "H", "H", "H", "H", "H", "H", "H", "H", "H"],
        vec!["8", "H", "H", "H", "H", "H", "H", "H", "H", "H", "H"],
        vec!["9", "H", "D", "D", "D", "D", "H", "H", "H", "H", "H"],
        vec!["10", "D", "D", "D", "D", "D", "D", "D", "D", "H", "H"],
        vec!["11", "D", "D", "D", "D", "D", "D", "D", "D", "D", "H"],
        vec!["12", "H", "H", "S", "S", "S", "H", "H", "H", "H", "H"],
        vec!["13", "S", "S", "S", "S", "S", "H", "H", "H", "H", "H"],
        vec!["14", "S", "S", "S", "S", "S", "H", "H", "H", "H", "H"],
        vec!["15", "S", "S", "S", "S", "S", "H", "H", "H", "H", "H"],
        vec!["16", "S", "S", "S", "S", "S", "H", "H", "H", "H", "H"],
        vec!["17", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S"],
        vec!["18", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S"],
        vec!["19", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S"],
        vec!["20", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S"],
        vec!["21", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S"]
    ];
    pub static ref STRAT_SOFT: Vec<Vec<&'static str>> = vec![
        vec!["0", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11"],
        vec!["13", "H", "H", "H", "D", "D", "H", "H", "H", "H", "H"],
        vec!["14", "H", "H", "H", "D", "D", "H", "H", "H", "H", "H"],
        vec!["15", "H", "H", "D", "D", "D", "H", "H", "H", "H", "H"],
        vec!["16", "H", "H", "D", "D", "D", "H", "H", "H", "H", "H"],
        vec!["17", "H", "D", "D", "D", "D", "H", "H", "H", "H", "H"],
        vec!["18", "S", "D", "D", "D", "D", "S", "S", "H", "H", "H"],
        vec!["19", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S"],
        vec!["20", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S"],
        vec!["21", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S"]
    ];
    pub static ref STRAT_SPLIT: Vec<Vec<&'static str>> = vec![
        vec!["0", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11"],
        vec!["2", "P", "P", "P", "P", "P", "P", "H", "H", "H", "H"],
        vec!["3", "P", "P", "P", "P", "P", "P", "H", "H", "H", "H"],
        vec!["4", "H", "H", "H", "P", "P", "H", "H", "H", "H", "H"],
        vec!["6", "P", "P", "P", "P", "P", "H", "H", "H", "H", "H"],
        vec!["7", "P", "P", "P", "P", "P", "P", "H", "H", "H", "H"],
        vec!["8", "P", "P", "P", "P", "P", "P", "P", "P", "P", "P"],
        vec!["9", "P", "P", "P", "P", "P", "S", "P", "P", "S", "S"],
        vec!["11", "P", "P", "P", "P", "P", "P", "P", "P", "P", "P"]
    ];
    pub static ref MAP_HARD: Vec<char> = vec_to_map(&STRAT_HARD);
    pub static ref MAP_SOFT: Vec<char> = vec_to_map(&STRAT_SOFT);
    pub static ref MAP_SPLIT: Vec<char> = vec_to_map(&STRAT_SPLIT);
}

pub fn get_action(player_val: i32, dealer_val: i32, strategy: &[char]) -> char {
    let key = player_val * 12 + dealer_val;
    strategy[key as usize]
}

pub fn vec_to_map(vec: &[Vec<&'static str>]) -> Vec<char> {
    let mut temp = vec![' '; 1000];
    for row in 0..vec.len() {
        for col in 0..vec[0].len() {
            let player_val = vec[row][0].parse::<i32>().unwrap();
            let dealer_val = vec[0][col].parse::<i32>().unwrap();
            let key = player_val * 12 + dealer_val;
            temp[key as usize] = vec[row][col].chars().next().unwrap();
        }
    }
    temp
}
