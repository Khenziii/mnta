{ pkgs ? import <nixpkgs> {}}:

let
  shared = import ./shared.nix { inherit pkgs; };
in 

pkgs.mkShell {
    inherit (shared) buildInputs nativeBuildInputs;
}
