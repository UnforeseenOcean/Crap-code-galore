#Persistent
#SingleInstance Force
loop{
Random, aaa, 0, 4
If (aaa == 0){
SoundPlay, *-1
}
If (aaa == 1){
SoundPlay, *16
}
If (aaa == 2){
SoundPlay, *32
}
If (aaa == 3){
SoundPlay, *48
}
If (aaa == 4){
SoundPlay, *64
}
sleep, 150
}
