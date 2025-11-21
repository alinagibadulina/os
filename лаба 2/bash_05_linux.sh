#!/bin/bash
mode=$1
file=$2
if [ -z "$mode" ]; then
  echo "Usage: $0 {создать|удалить} {имя_файла}"
  exit 0
fi
if [ -z "$file" ]; then
  echo "Не указано имя файла!"
  exit 0
fi
case "$mode" in
  создать)
    if [ -e "$file" ]; then
      echo "Файл $file уже существует!"
    else > "$file"
      echo "Файл $file создан."
    fi;;
  удалить)
    if [ -e "$file" ]; then
      rm "$file"
      echo "Файл $file удалён."
    else
      echo "Файл $file отсутствует!"
    fi;;
  *)
    echo "Неверный режим: используйте 'создать' или 'удалить'";;
esac
