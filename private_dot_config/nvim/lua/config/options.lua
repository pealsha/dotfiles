-- Options are automatically loaded before lazy.nvim startup
-- Default options that are always set: https://github.com/LazyVim/LazyVim/blob/main/lua/lazyvim/config/options.lua
-- Add any additional options here

-- Use 4 spaces for indentation
vim.opt.tabstop = 4
vim.opt.shiftwidth = 4
vim.opt.softtabstop = 4
vim.opt.expandtab = true

-- Send copies to the connecting terminal in WSL and SSH sessions.
-- NVIM_OSC52=0 disables this provider; NVIM_OSC52=1 also enables it in
-- existing tmux panes whose shell does not have SSH environment variables.
local use_osc52 = vim.env.NVIM_OSC52 == "1"
  or (
    vim.env.NVIM_OSC52 ~= "0"
    and (vim.fn.has("wsl") == 1 or vim.env.SSH_TTY ~= nil or vim.env.SSH_CONNECTION ~= nil)
  )
if use_osc52 then
  vim.opt.clipboard = "unnamedplus"
  local osc52 = require("vim.ui.clipboard.osc52")
  local copied = { { "" }, "v" }
  local function copy(register)
    local send = osc52.copy(register)
    return function(lines, regtype)
      copied = { vim.deepcopy(lines), regtype }
      send(lines, regtype)
    end
  end
  -- Never query the terminal's clipboard: unsupported queries can block p.
  -- Use terminal paste (e.g. Ctrl+Shift+V in Insert mode) for OS contents.
  local function paste()
    return vim.deepcopy(copied)
  end
  vim.g.clipboard = {
    name = "OSC 52 (copy only)",
    copy = {
      ["+"] = copy("+"),
      ["*"] = copy("*"),
    },
    paste = {
      ["+"] = paste,
      ["*"] = paste,
    },
  }
end
