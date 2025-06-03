{ pkgs ? import <nixpkgs> {}}:

let
  shared = import ./shared.nix { inherit pkgs; };
in 

pkgs.mkShell {
    inherit (shared) buildInputs nativeBuildInputs;

    shellHook = ''
        export PKG_CONFIG_PATH="$PKG_CONFIG_PATH:${pkgs.cjson}/lib/pkgconfig"
    '';
}
