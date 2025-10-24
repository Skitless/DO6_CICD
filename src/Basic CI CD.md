# Basic CI/CD

## Part 1. Настройка **gitlab-runner**

Подними виртуальную машину *Ubuntu Server 22.04 LTS*.

![1.png](Basic%20CI%20CD/1.png)

*Будь готов, что в конце проекта нужно будет сохранить дамп образа виртуальной машины.*

Вставляем команду для установки репозитория ссылка на гайд по установке [[тык](https://www.dmosk.ru/miniinstruktions.php?mini=gitlab-runner-web)]

```bash
curl -L "https://packages.gitlab.com/install/repositories/runner/gitlab-runner/s
cript.deb.sh" | sudo bash
```

Устанавливаем gitlab runner по ссылке [[тык](https://docs.gitlab.com/runner/install/linux-manually.html)] используя бинарник

```bash
# Linux x86-64
sudo curl -L --output /usr/local/bin/gitlab-runner "https://s3.dualstack.us-east-1.amazonaws.com/gitlab-runner-downloads/latest/binaries/gitlab-runner-linux-amd64"
```

Добавляем права на исполнение

```bash
sudo chmod +x /usr/local/bin/gitlab-runner
```

Добавляем пользователя

```bash
sudo useradd --comment 'GitLab Runner' --create-home gitlab-runner --shell /bin/bash
```

Устанавливаем и запускаем как службу

```bash
sudo gitlab-runner install --user=gitlab-runner --working-directory=/home/gitlab-runner
sudo gitlab-runner start
```

![7.png](Basic%20CI%20CD/7.png)

Скачай и установи на виртуальную машину **gitlab-runner**.

Запусти **gitlab-runner** и зарегистрируй его для использования в текущем проекте (*DO6_CICD*).

![4.png](Basic%20CI%20CD/4.png)

Ключ на платформе

![5.png](Basic%20CI%20CD/5.png)

Для регистрации пишем gitlab-runner register

![3.png](Basic%20CI%20CD/3.png)

## Part 2. Сборка

Напиши этап для **CI** по сборке приложений из проекта *C2_SimpleBashUtils*.

В файле *gitlab-ci.yml* добавь этап запуска сборки через мейк файл из проекта *C2*.

![6.png](Basic%20CI%20CD/6.png)

Файлы, полученные после сборки (артефакты), сохрани в произвольную директорию со сроком хранения 30 дней.

## Part 3. Тест кодстайла

Если кодстайл не прошел, то «зафейли» пайплайн.

В пайплайне отобрази вывод утилиты *clang-format*.

![9.png](Basic%20CI%20CD/9.png)

![10.png](Basic%20CI%20CD/10.png)

## Part 4. Интеграционные тесты

Напиши этап для **CI**, который запускает твои интеграционные тесты из того же проекта.

Запусти этот этап автоматически только при условии, если сборка и тест кодстайла прошли успешно.

Если тесты не прошли, то «зафейли» пайплайн.

В пайплайне отобрази вывод, что интеграционные тесты успешно прошли / провалились.

![11.png](Basic%20CI%20CD/11.png)

![12.png](Basic%20CI%20CD/12.png)

![13.png](Basic%20CI%20CD/13.png)

Для грепа прошли не все тесты, так как сборка и тестирование было на линуксе, а разработка на мак ос, и на мак все тесты проходят

## Part 5. Этап деплоя

Подними вторую виртуальную машину *Ubuntu Server 22.04 LTS*.

![18.png](Basic%20CI%20CD/18.png)

Настраиваем внутреннюю сеть

![15.png](Basic%20CI%20CD/15.png)

![14.png](Basic%20CI%20CD/14.png)

Пингуем виртуальные машины

![16.png](Basic%20CI%20CD/16.png)

![17.png](Basic%20CI%20CD/17.png)

Напиши этап для **CD**, который «разворачивает» проект на другой виртуальной машине.

Запусти этот этап вручную при условии, что все предыдущие этапы прошли успешно.

Генерируем ssh ключи для двух машин

![21.png](Basic%20CI%20CD/21.png)

Настраиваем аутентификацию по ssh чтобы не вводить пароль на машинах

`ssh-copy-id user@addres`

![23.png](Basic%20CI%20CD/23.png)

Добавляем ключ от второй машины в gitlab

![22.png](Basic%20CI%20CD/22.png)

Установка ssh агента

Открываем `etc/gitlab-runner/config.toml` и добавляем

```bash
environment = ["SSH_AUTH_SOCK=/tmp/ssh-agent"]
```

![24.png](Basic%20CI%20CD/24.png)

Затем сохраняем публичный ключ в файл `known_host`

![25.png](Basic%20CI%20CD/25.png)

Чтобы у ранера не было пробрем с поиском приватного ключа мы его скопируем в домашний каталог gitlab-runner

![27.png](Basic%20CI%20CD/27.png)

Так как мы под рутом, нужно поменять имена пользователся с помощью команды

```bash
chown gitlab-runner:gitlab-runner id_rsa known_hosts
```

![26.png](Basic%20CI%20CD/26.png)

Напиши bash-скрипт, который при помощи **ssh** и **scp** копирует файлы, полученные после сборки (артефакты), в директорию */usr/local/bin* второй виртуальной машины.

![19.png](Basic%20CI%20CD/19.png)

В файле *gitlab-ci.yml* добавь этап запуска написанного скрипта.

![20.png](Basic%20CI%20CD/20.png)

В случае ошибки «зафейли» пайплайн.

В результате ты должен получить готовые к работе приложения из проекта *C2_SimpleBashUtils* (s21_cat и s21_grep) на второй виртуальной машине.

Результат запуска

![29.png](Basic%20CI%20CD/29.png)

![28.png](Basic%20CI%20CD/28.png)

Сохрани дампы образов виртуальных машин.

## Part 6. Дополнительно. Уведомления

Настрой уведомления об успешном/неуспешном выполнении пайплайна через бота с именем «[твой nickname] DO6 CI/CD» в *Telegram*.

- Текст уведомления должен содержать информацию об успешности прохождения как этапа **CI**, так и этапа **CD**.
- В остальном текст уведомления может быть произвольным.

Создаем бота через BotFather

![31.png](Basic%20CI%20CD/31.png)

Для работы бота в yml файле надо добавить after script в каждой стадии

```bash
after_script:
    - |
      if [ "$CI_JOB_STATUS" == "success" ]; then
        curl -X POST "https://api.telegram.org/bot<bot_token>/sendMessage" -d "chat_id=<chat_id>" -d "text=Этап деплоя завершен успешно!"
      else
        curl -X POST "https://api.telegram.org/bot<bot_token>/sendMessage" -d "chat_id=<chat_id>" -d "text=Этап деплоя завершен с ошибкой!"
      fi
```

Результат работы бота

![32.png](Basic%20CI%20CD/32.png)
