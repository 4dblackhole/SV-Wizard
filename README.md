# SV-Wizard

This is a tool helps making osu SV objects.
It reads .osu file and performs interpolation to generate green lines have appropriate speed for each notes.

When you press "Generate" button, this application reads datas from .osu file, and generate objects.
so you don't need to concern about incoherence between memory area of this application and existing .osu file.
But if you want to check the changes, press ctrl+L frequently on osu! Editor.

There are 3 ways of interpolation.
1) Linear
2) Exponential
3) Focusing

Linear and Exponential types are interpolations in a literal way.
Linear) 1.0 -> 1.1 -> 1.2 -> 1.3
Exponential) 1.0 -> 1.2 -> 1.44 -> 1.728

The term Focusing is derived from Focus.
Because when you use this option, all of notes passes the specified point at the same time.
And the fomula is harmonic progression.
Focusing) 1.0 -> 1.2 -> 1.5 -> 2.0 -> 3.0

Each Kiai, Volume option has "Auto".
If you don't want to change the existing Volume and Kiai settings, you may want to enable this option.
Or else, you can change the Volume/Kiai settings of all green lines. (but it doesn't apply on existing lines. you should do extra operations.)
