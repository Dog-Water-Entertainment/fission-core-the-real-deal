s* 
author: b martinez
purpose: deals with hp for all characters/entities
date: 9/11/24
*/

#pragma once
#include "hptype.h"
class hp {
public:
    //returns true if set successfully
    bool setMaxHP (hptype new_max_hp) {
    if (new_max_hp < 1)
	return false;
    if (CurrentHP < new_max_hp)
	CurrentHP = new_max_hp;
    return true;
    }

    bool takeDmg (hptype dmg_taken) {
    


private:
    hptype CurrentHP;
    hptype MaxHp;
	
};
