-- Options are automatically loaded before lazy.nvim startup
-- Default options that are always set: https://github.com/LazyVim/LazyVim/blob/main/lua/lazyvim/config/options.lua
-- Add any additional options here

-- Use 4 spaces for indentation
vim.opt.tabstop = 4
vim.opt.shiftwidth = 4
vim.opt.softtabstop = 4
vim.opt.expandtab = true

-- Sync Neovim clipboard with the Windows clipboard when running in WSL.
if vim.fn.has("wsl") == 1 then
  vim.opt.clipboard = "unnamedplus"
  vim.g.clipboard = {
    name = "WslClipboard",
    copy = {
      ["+"] = "clip.exe",
      ["*"] = "clip.exe",
    },
    paste = {
      ["+"] = { "powershell.exe", "-NoProfile", "-Command", "Get-Clipboard" },
      ["*"] = { "powershell.exe", "-NoProfile", "-Command", "Get-Clipboard" },
    },
    cache_enabled = 0,
  }
end
