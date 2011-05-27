<?php

/**
 * @file
 * Recurly settings forms and administration page callbacks.
 */


/**
 * Returns the site-wide Recurly settings form.
 */
function recurly_settings_form($form, &$form_state) {
  $form['account'] = array(
    '#type' => 'fieldset',
    '#title' => t('Default account settings'),
    '#description' => t('Configure these settings based on your Company Settings and API Credentials settings in the Recurly administration interface.'),
  );
  $form['account']['recurly_api_username'] = array(
    '#type' => 'textfield',
    '#title' => t('Username'),
    '#default_value' => variable_get('recurly_api_username', ''),
  );
  $form['account']['recurly_api_password'] = array(
    '#type' => 'textfield',
    '#title' => t('Password'),
    '#default_value' => variable_get('recurly_api_password', ''),
  );
  $form['account']['recurly_subdomain'] = array(
    '#type' => 'textfield',
    '#title' => t('Subdomain'),
    '#description' => t("The subdomain of your account excluding the -test suffix if still in Sandbox."),
    '#default_value' => variable_get('recurly_subdomain', ''),
  );
  $form['account']['recurly_environment'] = array(
    '#type' => 'radios',
    '#title' => t('Environment'),
    '#description' => t('Select the environment that matches your account status.'),
    '#options' => array(
      'sandbox' => t('Sandbox'),
      'production' => t('Production'),
    ),
    '#default_value' => variable_get('recurly_environment', 'sandbox'),
  );

  return system_settings_form($form);
}

/**
 * Displays a list of subscription plans currently defined in your Recurly account.
 */
function recurly_subscription_plans_overview() {
  // Initialize the Recurly client with the site-wide settings.
  if (!recurly_client_initialize()) {
    return t('Could not initialize the Recurly client.');
  }

  // Retrieve the plans for the current account.
  $plans = recurly_subscription_plans();

  // Format the plan data into a table for display.
  $header = array(t('Subscription plan'), t('Price'), t('Setup fee'), t('Trial'), t('Created'));
  $rows = array();

  foreach ($plans as $plan) {
    // Prepare the description string if one is given for the plan.
    if (!empty($plan->description)) {
      $description = '<br />' . check_plain($plan->description);
    }
    else {
      $description = '';
    }

    $rows[] = array(
      '<strong>' . check_plain($plan->name) . '</strong> ' . check_plain($plan->plan_code) . $description,
      t('@unit_price per @interval_length @interval_unit', array('@unit_price' => number_format($plan->unit_amount_in_cents / 100, 2), '@interval_length' => $plan->plan_interval_length, '@interval_unit' => $plan->plan_interval_unit)),
      t('@setup_fee', array('@setup_fee' => number_format($plan->setup_fee_in_cents / 100, 2))),
      t('@trial_length @trial_unit', array('@trial_length' => $plan->trial_interval_length, '@trial_unit' => $plan->trial_interval_unit)),
      format_date($plan->created_at),
    );
  }

  if (empty($rows)) {
    $rows[] = array(array('data' => t('No subscription plans found.'), 'colspan' => 5));
  }

  return theme('table', array('header' => $header, 'rows' => $rows));
}
