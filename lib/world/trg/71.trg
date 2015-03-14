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
if %actor.is_pc% && (%actor.level% >= 32)
  return 1
  halt
end
*
* We're not an imm, so deal damage
*
set destination 7122
set health %actor.hitp%
set removeHP 0
*
* Now move the victim
*
%echo% Move the victim
%teleport% %actor% %destination%
*
if %health% > 20
  * This is survivable
  eval removeHP %actor.hitp% - 10
  %echo% Deal %removeHP% damage to %actor.name%
  %damage% %actor% %removeHP%
  %echoaround% %actor% %actor.name% plumets in from above and lands with a crunch
  %send% %actor% That was close, you barely survived that fall.
else
  * This is fatal
  eval removeHP %actor.hitp% * 2
  %echo% Deal %removeHP% damage to %actor.name%
  %damage% %actor% %removeHP%
  %echoaround% %actor% %actor.name% plumets in from above and lands with a wet @rsplat@n
  %send% %actor% Watch your step next time.
end
%echo% %actor.name% HP = %actor.hitp%
*
* Now move any objects
*
*%echo% Move any objects to the destination
*%move% all %destination%
eval this_room %self%
eval next_room %this_room.vnum%+1
eval in_rm %this_room.contents%
while %in_rm%
  set next_in_rm %in_rm.next_in_list%
  %echo% DBG: Move obj %in_rm.id%:%in_rm.vnum%:%in_rm.name%
  %move% %in_rm% %destination%
  set in_rm %next_in_rm%
done
~
$~
