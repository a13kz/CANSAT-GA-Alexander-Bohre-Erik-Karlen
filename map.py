import folium
import sys

m = folium.Map(location=(-71.351871840295871, -73.655963711222626), zoom_start=11)
path = 'coordinates'
sond_coordinates = [
    (-71.351871840295871, -73.655963711222626),
    (-71.374144382613707, -73.719861619751498),
    (-71.391042575973145, -73.784922248007007),
    (-71.400964450973134, -73.851042243124397),
    (-71.402411391077322, -74.050048183880477),
]

folium.PolyLine(sond_coordinates, tooltip="Sond_position").add_to(m)
print("Name reading:")
name = input()

m.save(f"maps/map_{name}.html")