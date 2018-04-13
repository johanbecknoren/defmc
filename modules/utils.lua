-- Put functions in this file to use them in several other scripts.
-- To get access to the functions, you need to put:
-- require "my_directory.my_file"
-- in any script using the functions.
local M = {}

-- Taken from: https://gist.github.com/pr0digy/1383576
function M.smooth(points, steps)

    if #points < 3 then
        return points
    end

    local steps = steps or 5
    local spline = {}
    local count = #points - 1
    local p0, p1, p2, p3, x, y

    for i = 1, count do
        if i == 1 then
            p0, p1, p2, p3 = points[i], points[i], points[i + 1], points[i + 2]
        elseif i == count then
            p0, p1, p2, p3 = points[#points - 2], points[#points - 1], points[#points], points[#points]
        else
            p0, p1, p2, p3 = points[i - 1], points[i], points[i + 1], points[i + 2]
        end 

        for t = 0, 1, 1 / steps do
            x = 0.5 * ( ( 2 * p1.x ) + ( p2.x - p0.x ) * t + ( 2 * p0.x - 5 * p1.x + 4 * p2.x - p3.x ) * t * t + ( 3 * p1.x - p0.x - 3 * p2.x + p3.x ) * t * t * t )
            y = 0.5 * ( ( 2 * p1.y ) + ( p2.y - p0.y ) * t + ( 2 * p0.y - 5 * p1.y + 4 * p2.y - p3.y ) * t * t + ( 3 * p1.y - p0.y - 3 * p2.y + p3.y ) * t * t * t )
            --prevent duplicate entries
            if not(#spline > 0 and spline[#spline].x == x and spline[#spline].y == y) then
                table.insert( spline , { x = x , y = y } )              
            end 
        end
    end 
    return spline
end

function M.normalizedTo2D(x, y, w_px, h_px)
    local abs_x = -1
    if x < 0 then
        abs_x = ofs_l
    elseif x > 1 then
        abs_x = w_px - ofs_r
    else
        abs_x = (w_px - ofs_r - ofs_l) * x + ofs_l
    end

    local abs_y = -1
    if y < 0 then
        abs_y = ofs_d
    elseif y > 1 then
        abs_y = h_px - ofs_u
    else
        abs_y = (h_px - ofs_u - ofs_d) * y + ofs_d
    end

    return math.floor(abs_x), math.floor(abs_y)
end

function M.twoDToLinear(x, y, w_px, entry_size)
    return ((y-1) * w_px + (x-1)) * entry_size + 1
end

function M.is_flip_input(dx, dy)
	--print("dx: ", dx, ", dy: ", dy)
	--return true
    if dx == 0 and dy == 0 then
        return false
    else
	   return math.abs(dx) >= (math.abs(dy) + 1.0*(math.abs(dy)))
    end
end

function M.calc_rot_target(src, dest)
    local res = dest
    
    if dest - src > 180 then
        dest = dest - 360
    end
    
    return dest
end

function M.roundToPrecision(val, digits)
    shift = 10 ^ digits
    return math.floor(val*shift + 0.5) / shift
end

return M