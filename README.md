# Masters-Project-PSoC-Labs
A Masters Project for Educational Laboratories for the PSoC 5LP (circa 2014), including a final project example that emulates the YM2149 Programmable Sound Generator.

# YM2149 TL;DR
If all you're here for is the the YM2149 Programmable Sound Generator emulator written for the Cypress PSoC 5LP, you can find it in [this folder](Source/FinalProject.cydsn).

IMPORTANT: Due to using the [emFile library](https://www.cypress.com/documentation/component-datasheets/file-system-library-emfile) to use an SD card to read music data, the emulator won't compile out of the box, nor is there a .hex provided. See the readme in the above folder for details.

There is a video demonstrating the project playing back a song from the Atari ST [here](https://www.youtube.com/watch?v=B2MvwRQ0q_I).

# Master's Project Information

## Title
Developing Multidisciplinary Laboratories with the Cypress PSoC

## Defense Date
May 23, 2014

## Abstract
There aren't many courses at RIT that combine both Computer Engineering and Computer Science. Because of that, it is hard to be multidisciplinary in computer hardware and software if you are interested in both. The goal of this project is to create educational labs using a hardware platform that combines software and hardware for multidisciplinary learning. These labs combine programming software and configuring hardware to develop example real-world applications, leading to a culminating project.

## Documents
* [Proposal](proposal.pdf)
* [Report](report.pdf)
* [Defense Presentation](defense.pdf)
* [Labs and Final Project](Course)
* [Source code for example lab solutions and final project](Source)
  * This includes the YM2149 Programmable Sound Generator emulator, which is the example final project
  * [Example of the YM2149 emulator in action](https://www.youtube.com/watch?v=B2MvwRQ0q_I)

Note: the e-mail address found in these documents is defunct.

# Disclaimer
This project was completed in 2014. As of the time I post this in 2020 (to the few who were asking for this, sorry for the wait), I have not tested to see if the source code is still functional in the current version of Cypress PSoC Creator. But everything relevant in the source code should be shown in the lab instructor's guides and final project example documents.

# License
The proposal, report, defense presentation, lab documents, final project documents, and all other documentation are licensed under the [Creative Commons Attribution 4.0 International License](https://creativecommons.org/licenses/by/4.0/). The source code not owned by Cypress, used for the example lab solutions and the YM2149 Programmable Sound Generator emulator, is licensed under the [BSD 3-Clause license](LICENSE.md).
