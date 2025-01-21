import pygame
import time


def parse_line(line):
    try:
        parts = line.split(";")
        time_parts = parts[1].split("$")
        hour = int(time_parts[0])
        minute = int(time_parts[1])
        second = int(time_parts[2])

        lat = float(parts[3].split("$")[0])
        lon = float(parts[3].split("$")[1])
        speed = float(parts[4])  # Geschwindigkeit in km/h

        return hour, minute, second, lat, lon, speed
    except (IndexError, ValueError):
        print(f"Ungültige Zeile: {line}")
        return None, None, None, None, None, None


def main():
    pygame.init()
    screen = pygame.display.set_mode((1000, 1000))
    screen.fill((0, 0, 0))  # Hintergrund einmalig schwarz füllen
    pygame.display.set_caption("Geodaten-Visualisierung")

    # Schriftart und Größe für Textanzeige
    font = pygame.font.Font(None, 36)

    lat_center = 0
    lon_center = 0
    centered = False
    last_lat_rest = 0
    last_lon_rest = 0

    text_area_rect = pygame.Rect(0, 0, 1000, 50)  # Rechteck für den Textbereich

    with open("Raw Data/0003.TXT", "r") as file:
        for line in file:
            hour, minute, second, lat, lon, speed = parse_line(line)
            if lat is None or lon is None:
                continue

            lat_rest = lat * 5000 * 5
            lon_rest = lon * 5000 * 5

            if not centered:
                lat_center = lat_rest
                lon_center = lon_rest
                centered = True
            else:
                # Linie zwischen Punkten zeichnen
                pygame.draw.line(
                    screen,
                    (255, 255, 255),  # Weiße Linie
                    (last_lat_rest - lat_center + 500, last_lon_rest - lon_center + 500),
                    (lat_rest - lat_center + 500, lon_rest - lon_center + 500),
                )

            last_lat_rest = lat_rest
            last_lon_rest = lon_rest

            # Zeit als hh:mm:ss formatieren
            time_display = f"{hour:02}:{minute:02}:{second:02}"

            # Textanzeige vorbereiten
            text_surface = font.render(
                f"Geschwindigkeit: {speed:.2f} km/h | Zeit: {time_display}",
                True,
                (255, 255, 255),
            )

            # Textbereich leeren (damit alter Text entfernt wird)
            pygame.draw.rect(screen, (0, 0, 0), text_area_rect)

            # Text über die Route legen
            screen.blit(text_surface, (10, 10))  # Text zeichnen
            pygame.display.flip()

            time.sleep(0.1)

    while True:
        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                pygame.quit()
                return


if __name__ == "__main__":
    main()
