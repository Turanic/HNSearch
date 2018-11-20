#!/bin/sh

FILE_URL="https://ucc1b98a77dbf55d60c26d106262.dl.dropboxusercontent.com/cd/0/get/AV__wr3WbI9zB_al8u0Sf42SXgwF_czTJwF3X2TNMBUGg7g-2jKRIUmCM9ggeBLYuydiJZcJ_mV44xnO1NjuEY3ftUgMare7DDkDXUjB6dEK9DSLTaG4Rn-fdWEaHD_a-R7VlKVUntYOCIaUEaHkVvEJrGccL_he0LF0DwPGhJ4iBXY9COoAGDG7gIm9kLUCglY/file?_download_id=2075967438629946523695299686264989998427670255199033388463735491314&_notify_domain=www.dropbox.com&dl=1"

wget -q -O hn_logs.tsv.tar.bz2 $FILE_URL &&\
tar -xvjf hn_logs.tsv.tar.bz2 &&\
./hnStat top 3 hn_logs.tsv
