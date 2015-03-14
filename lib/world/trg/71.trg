#7100
Near Death Trap Mid-Air - 7114, 7190~
2 g 100
~
* Near Death Trap stuns actor
wait 1 sec
set stunned %actor.hitp% 
%damage% %actor% %stunned%
wait 5 sec
%send% %actor% Watch your step next time.
~
#7101
Near Death Trap Mid-Air - 7114, 7190~
2 g 100
~
*
* Near Death Trap stuns actor
*
* Sanity check for hidden (nohassle) immortals
*DBG* %echo% DBG: Check for hidden (nohassle) players
eval plrname %actor.name%
eval namelen %plrname.strlen%
if %namelen% <= 1
  return 1
  halt
end
*
*DBG* %echo% DBG: Check for player and level
if %actor.is_pc% && (%actor.level% >= 32)
  %send% %actor% As you step out into the air glowing platforms of energy appear beneath your feet.
  %echoaround% %actor% Glowing patforms of energy appear beneath %actor.name%'s feet as %actor.heshe% steps out into the air.
  return 1
  halt
end
*
* We're not an imm, so deal damage
*
*DBG* %echo% DBG: Determine destination based on room
* For now default to midgaard dump
eval destination 3030
if %self.vnum(7190)%
  eval destination 7118
elseif %self.vnum(7114)%
  eval destination 7119
end
*DBG* %echo% DBG: source VNUM = %self.vnum%, destination VNUM is %destination%
set health %actor.hitp%
set removeHP 0
*
* Now move the victim
*
*DBG* %echo% DBG: Move the victim
%teleport% %actor% %destination%
*
if %health% > 20
  * This is survivable
  eval removeHP %actor.hitp% - 10
  *DBG* %echo% DBG: Deal %removeHP% damage to %actor.name%
  %damage% %actor% %removeHP%
  %echoaround% %actor% %actor.name% plumets in from above and lands with a crunch
  %send% %actor% That was close, you barely survived that fall.
else
  * This is fatal
  eval removeHP %actor.maxhitp% * 2
  *DBG* %echo% DBG: Deal %removeHP% damage to %actor.name%
  %damage% %actor% %removeHP%
  %echoaround% %actor% %actor.name% plumets in from above and lands with a wet splat
  %send% %actor% Watch your step next time.
end
*DBG* %echo% DBG: %actor.name% HP = %actor.hitp%
*
* Now move any objects
*
*DBG* %echo% DBG: Move any objects to the destination
%move% all %destination%
*
~
$~
