cd ~/nuclear/be9/summed/

make clean
make -j 4 || ( echo -e "\n\n ********************** \n\n" ; make clean ; make ; paplay ~/Music/Powerup.ogg ; return 1 )
9Be8Be run.mac exit.mac
rt runryan.C || return 1
rt finalrunryan.C
cp final_event.root onandon/final_event_
