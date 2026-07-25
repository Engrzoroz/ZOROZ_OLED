# Publishing ZOROZ OLED

## 1. Personalise metadata

Edit `library.properties`:

```properties
maintainer=Engr Zoroz <YOUR_REAL_EMAIL>
url=https://github.com/YOUR_USERNAME/ZOROZ_OLED
```

Edit the same repository URL in `library.json`.

## 2. Test hardware

Test at least:

- SH1106 128x64 at address 0x3C
- SSD1306 128x64 at address 0x3C
- Arduino Uno or Nano
- Arduino Mega
- ESP32

Run every example from the Arduino IDE Examples menu.

## 3. Create the GitHub repository

Repository name:

```text
ZOROZ_OLED
```

The repository root must directly contain:

```text
library.properties
README.md
LICENSE
src/
examples/
```

Do not put the library inside an additional nested folder in the repository.

## 4. Create release 1.0.0

- Commit all tested files.
- Create tag `1.0.0`.
- Create GitHub release `ZOROZ OLED 1.0.0`.
- Confirm `library.properties` also contains `version=1.0.0`.

## 5. Run Arduino Lint

From the repository root:

```bash
arduino-lint --library-manager submit
```

Fix all errors before submission. Warnings should also be reviewed.

## 6. Submit to Arduino Library Manager

- Fork `arduino/library-registry`.
- Create a branch such as `add-zoroz-oled`.
- Add the ZOROZ OLED repository homepage URL to `repositories.txt`.
- Commit only that URL addition.
- Open a pull request to `arduino/library-registry` main.
- Follow ArduinoBot and maintainer comments.

## 7. Future releases

For a bug fix:

```properties
version=1.0.1
```

For backward-compatible new features:

```properties
version=1.1.0
```

For breaking API changes:

```properties
version=2.0.0
```

Create a matching Git tag for every public version. After initial registry acceptance, new compliant tagged versions are indexed automatically.
