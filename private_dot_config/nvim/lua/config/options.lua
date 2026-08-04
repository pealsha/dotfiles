-- Options are automatically loaded before lazy.nvim startup
-- Default options that are always set: https://github.com/LazyVim/LazyVim/blob/main/lua/lazyvim/config/options.lua
-- Add any additional options here

-- Use 4 spaces for indentation
vim.opt.tabstop = 4
vim.opt.shiftwidth = 4
vim.opt.softtabstop = 4
vim.opt.expandtab = true

-- Sync Neovim clipboard with the system clipboard via OSC 52 escape sequences.
-- Works over WSL/SSH regardless of WSL interop; the terminal emulator relays
-- yanks to the Windows clipboard. Paste from the OS clipboard depends on
-- terminal support (Windows Terminal doesn't reply), so use the terminal's own
-- paste (Ctrl+Shift+V) to insert OS clipboard contents into Neovim.
if vim.fn.has("wsl") == 1 then
  vim.opt.clipboard = "unnamedplus"
  local osc52 = require("vim.ui.clipboard.osc52")
  vim.g.clipboard = {
    name = "OSC 52",
    copy = {
      ["+"] = osc52.copy("+"),
      ["*"] = osc52.copy("*"),
    },
    paste = {
      ["+"] = osc52.paste("+"),
      ["*"] = osc52.paste("*"),
    },
  }
end
