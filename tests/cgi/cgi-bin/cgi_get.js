/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cgi_get.js                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maricard <maricard@student.porto.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/18 17:24:21 by maricard          #+#    #+#             */
/*   Updated: 2023/11/12 12:18:17 by maricard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// Get current date and time
const dt = new Date();

// Format to dd/mm/YYYY H:M:S
const DateOptions = {
  day: '2-digit',
  month: '2-digit',
  year: 'numeric'
};

const TimeOptions = {
	hour: '2-digit',
	minute: '2-digit',
	second: '2-digit',
	hour12: false
  };

const formattedDT = dt.toLocaleDateString('en-GB', DateOptions) + ' ' + 
					dt.toLocaleTimeString('en-GB', TimeOptions);

const message = formattedDT;

console.log(message);
