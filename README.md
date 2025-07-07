<h1 align="center">MNTA</h1>

MNTA (minimalistic note-taking app) is a simple GTK app written in C that allows you to organize your notes and interact with them all from your keyboard. It's configurable and simple.

![Demo](./docs/output.gif)

## Configuration

By default your `~/.config/mnta/config.json` will look like this:

```json
{
    "editor_launch_command": "nvim",
    "background_color_hex": "#171617",
    "keyboard_navigation_sensitivity": 25,
    "default_files_placement": {
        "files_per_row": 5,
        "margin_horizontal": 300,
        "margin_vertical": 100,
        "container_padding": 50
    }
}
```

It should be pretty self-explanatory.

## Contributing

If you'd like to add any features, go ahead! This repository will be maintained, and your PR will be reviewed. I also encourage opening new issues if you feel like there's room for improvement.
